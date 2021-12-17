# Coding Style of Lorr

Lorr is written using combination of different things. This documentation will guide you to it's coding style. Firstly, [Orthodox C++](https://gist.github.com/bkaradzic/2e39896bc7d8c34e042b). Well, it's not completely Orthodox C++ but using some logic of it. Secondly, moving to EASTL is still in plan to gain ability read STL functions (that will help to programmer's life in debugging).

# Naming Conventions
    
```cpp
/// Macros
#define RENDERER_API_D3D11 1
#define RENDERER_API_D3D12 0

/// Pointers
uint8_t *pMemory;
Player *pPlayer;
uintptr_t *pMemPos;

/// Comments
/// Title
int x; // Note
//* Highlighted Title *//

//?--------------------------- // NOTE: if really needed
void DoStuffRelatedToX();
void DoDifferentStuffRelatedToX();
//?---------------------------
void DoStuffRelatedToY();
void DoDifferentStuffRelatedToY();
```

- Classes:
```cpp
class FriendClass;
class MyClass
{
public:
    MyClass() = default;
    void Init();

public:
    int GetX() 
    {
        return m_X;
    }

    // reference has to be used of we are getting/setting 
    // class/struct or even stuff like STL functions(string, vector, ...).

private: // or protected
    int m_X = 55;

    friend FriendClass;
};
```

- POD structs:
```cpp
struct MyStruct
{
    int myInt = 0;
    MyClass myClass;
};
```

- Structs with functions (`private` should not be used):
```cpp
struct MyStruct
{
    MyStruct(int x) : m_X(x)
    {
        ProcessData();
    }

    void ProcessData() 
    {
        // ...
        m_X++;
        // ...
    }

    int m_X = 0;
};
```

- Local variables:
```cpp
void ProcessData()
{
    FileHeader fileHeader;
    int currentOffset;
    float playerPos;

    // ...
}
```

- Trivial variables:
```cpp
for (int i = 0; ...) {}
for (int j = 0; ...) {}
for (int n = 0; ...) {}
for (int e = 0; ...) {}
```

- Global variables:
```cpp
extern MyClass *g_pMyClass;

static const std::string kDefaultPlayerName = "Alex";
constexpr std::string_view kFileSignature = "Lorr";
```

- Enums:
```cpp
enum : int // type has to be defined
{
    TEXTURE_TYPE_RGBA8,
    // ...
};

enum class ResourceType : uint8_t
{
    Audio,
    Texture,
    Model
};
```

- Functions:
```cpp
bool SetPlayerPos(Player *pPlayer, int posX, int posY, PlayerRotation rotation);
```

# Other stuff

- `auto` keyword should not be abused, it's ok to use on certain situations like `auto [transform, renderable] = Get<Transform, Renderable>` or `auto longAssClass` instead of `LongAssClassNameThatIsLongForSomeReason *pLong...`.
- 2 phase construction is used.
- Interface classes: `BaseMyClass` (before it was `IMyClass`).
- C++ header file: `MyHeaderFile.hh`
- C++ source file: `MySourceFile.cc`