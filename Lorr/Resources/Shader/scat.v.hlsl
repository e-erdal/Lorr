//$TYPE v
#define FLOAT_MAX 3.402823466e+38F

cbuffer cb {
  // camera
  matrix mvp;
  float3 cameraPosition;
  float cameraHeight2;

  float atmosRadius, atmosRadius2;
  float earthRadius, earthRadius2;
  // 1 / (atmosRadius - earthRadius)
  float invAtmosHeight;

  // float KrSun, KmSun;
  float3 lightDirection;
  float3 betaR, betaM;
  float3 sunIntensity;
  float g;
  float samples;
}

struct input {
  float4 Pos : POSITION;
  float3 Normal : NORMAL0;
  float2 UV : TEXCOORD0;
  float4 Color : COLOR0;
};

struct output {
  float4 colorR : COLOR0;
  float4 colorM : COLOR1;
  float4 intersect : COLOR3;
  float3 texCoord0 : TEXCOORD0;
  float2 texCoord1 : TEXCOORD1;
  float4 position : SV_POSITION;
};

struct s3Ray {
  float3 origin;
  float3 direction;
};

struct s3Sphere {
  float3 origin;
  float radius;
};

Texture2D lut;
sampler lutSampler;

bool solveQuadratic(float A, float B, float C, inout float t0, inout float t1) {
  // --!Copied from Mitsuba(util.cpp 447)
  /* Linear case */
  if (A == 0) {
    if (B != 0) {
      t0 = t1 = -C / B;
      return true;
    }
    return false;
  }

  float discrim = B * B - 4.0f * A * C;

  /* Leave if there is no solution */
  if (discrim < 0)
    return false;

  float temp, sqrtDiscrim = sqrt(discrim);

  /* Numerically stable version of (-b (+/-) sqrtDiscrim) / (2 * a)
   *
   * Based on the observation that one solution is always
   * accurate while the other is not. Finds the solution of
   * greater magnitude which does not suffer from loss of
   * precision and then uses the identity x1 * x2 = c / a
   */
  // 通过B的状态求得更大的temp来避免出现精度丢失问题
  if (B < 0)
    temp = -0.5f * (B - sqrtDiscrim);
  else
    temp = -0.5f * (B + sqrtDiscrim);

  t0 = temp / A;
  t1 = C / temp;

  /* Return the results so that t0 < t1 */
  if (t0 > t1) {
    float t = t0;
    t0 = t1;
    t1 = t;
  }

  return true;
}

// world space
bool raySphereIntersect(s3Ray r, s3Sphere s, inout float nearT,
                        inout float farT) {
  float3 o = r.origin - s.origin;
  float3 d = r.direction;

  // direction为单位向量，长度为1
  float A = length(r.direction);
  float B = 2 * dot(o, r.direction);
  float C = dot(o, o) - s.radius * s.radius;

  if (!solveQuadratic(A, B, C, nearT, farT))
    return false;

  return true;
}

float4 getOpticalDepth(float heightRatio, float cosTheta) {
  // Normalize cosAngle to[0, 1]
  // [x, y]->Rayleigh [z, w]->Mie
  // [x, z]->Atmospheric density [y, w]->Optical depth
  return lut.SampleLevel(lutSampler, float2(heightRatio, 0.5f - 0.5 * cosTheta),
                         0);
}

output main(input i) {
  // output
  output o;

  // init the ray
  s3Ray ray = {cameraPosition, normalize(i.Pos.xyz - cameraPosition)};
  s3Sphere atmos = {float3(0, 0, 0), atmosRadius};
  s3Sphere earth = {float3(0, 0, 0), earthRadius};

  float nearT = -FLOAT_MAX, farT = FLOAT_MAX;
  // Ray's start / end positions in the atmosphere
  raySphereIntersect(ray, atmos, nearT, farT);

  float nearT2 = -FLOAT_MAX, farT2 = FLOAT_MAX;
  o.intersect = float4(0, 0, 0, 0);
  if (raySphereIntersect(ray, earth, nearT2, farT2)) {
    o.intersect = float4(1, 1, 1, 1);
    farT = nearT2;
  }
  // numeric integration
  float segmentLength = (farT - nearT) / samples;
  float3 samplePoint =
      cameraPosition + ray.direction * (nearT + segmentLength * 0.5f);

  float4 lightOpticalLength, cameraOpticalLength;
  float3 sumR = float3(0, 0, 0), sumM = float3(0, 0, 0);

  for (int j = 0; j < samples; j++) {
    float height = length(samplePoint);
    float heightRatio = (height - earthRadius) * invAtmosHeight;

    if (heightRatio < 0)
      break;

    // for integral's ds
    float cosLight = dot(-lightDirection, samplePoint) / height;
    float cosCamera = dot(-ray.direction, samplePoint) / height;

    // LUT
    lightOpticalLength = getOpticalDepth(heightRatio, cosLight);
    cameraOpticalLength = getOpticalDepth(heightRatio, cosCamera);

    // [y, w]([Rayleigh, Mie]) contains optical depth
    float3 attenuation =
        exp(-betaR * (lightOpticalLength.y + cameraOpticalLength.y) -
            betaM * (lightOpticalLength.w + cameraOpticalLength.w));

    // [x, z]([Rayleigh, Mie]) contains atmospheric density at that altitude
    sumR += attenuation * lightOpticalLength.x * segmentLength * invAtmosHeight;
    sumM += attenuation * lightOpticalLength.z * segmentLength * invAtmosHeight;

    samplePoint += ray.direction * segmentLength;
  }

  // column-major matrix(transpose of the matrix on CPU)
  o.position = mul(float4(i.Pos.xyz, 1.0f), mvp);
  o.texCoord0 = normalize(cameraPosition - i.Pos.xyz);
  o.texCoord1 = i.UV;
  o.colorR = float4(sumR * sunIntensity * betaR, 1.0f);
  o.colorM = float4(sumM * sunIntensity * betaM, 1.0f);

  return o;
}