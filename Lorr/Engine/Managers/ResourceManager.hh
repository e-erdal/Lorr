//
// Created on Wednesday 22nd September 2021 by e-erdal
//

#pragma once

#include "Engine/Resource/IResource.hh"
#include "Engine/Resource/ResourceFile.hh"

#include "Engine/Graphics/Texture2D.hh"

#include "Engine/Stream/BufferStream.hh"
#include "Engine/Stream/FileStream.hh"

namespace Lorr
{
    class ResourceManager
    {
    public:
        ResourceManager() = default;

        void Init();

        // Prepared resource, we have everything we need without any file headers
        template<typename T = IResource<>>
        T *LoadPrepared(const Identifier &ident, IRESOURCE_DESC *pDesc, IRESOURCE_DESC_SUBRESC *pSubResc)
        {
            T *resource = new T;
            resource->Init(ident, pDesc, pSubResc);

            m_Resources.emplace(ident, resource);
            return resource;
        }

        // At the first stage, resource will be parsed due to it has engine's file type
        template<typename T = IResource<>>
        T *LoadFromFile(const Identifier &ident, IRESOURCE_DESC *pDesc, const std::string &path)
        {
            switch (T::m_Type)
            {
                case ResourceType::Texture: return ParseTexture(ident, (TEXTURE2D_DESC *)pDesc, path);
            }
        }

        template<typename T>
        void MakeResourceFromFile(const std::string &path, BufferStream &buf)
        {
            BufferStream compBuf;
            T *t = ParseFromFile<T>(path, compBuf);

            ResourceHeader header;
            header.Type = T::m_Type;
            // header.Flags |= RESOURCE_FILE_FLAGS_COMPRESSED;
            header.OriginalSize = compBuf.GetSize();

            // compBuf.Compress();
            buf.Insert(header);
            buf.InsertPtr(compBuf.GetData(), compBuf.GetSize());
        }

        // Parse out resource data
        template<typename T>
        T *ParseFromFile(const std::string &path, BufferStream &buf)
        {
            switch (T::m_Type)
            {
                case ResourceType::Texture:
                {
                    Texture2D *resource = new Texture2D;

                    FileStream f(path, false);
                    if (!f.IsOK())
                    {
                        return 0;
                    }

                    uint8_t *fileData = f.ReadAll<uint8_t>();
                    size_t fileLen = f.Size();
                    f.Close();

                    TEXTURE2D_DESC_SUBRESC sresc;
                    sresc.Data = fileData;
                    sresc.DataSize = fileLen;

                    buf.InsertZero(sizeof(TEXTURE2D_DESC_SUBRESC) - sizeof(size_t));
                    resource->ParseMemory(&sresc, buf);

                    buf.StartOver();
                    buf.Assign(sresc.Width);
                    buf.Assign(sresc.Height);
                    buf.Assign(sresc.Format);
                    buf.Assign(sresc.DataSize);

                    free(fileData);
                    return resource;
                }
            }
        }

    public:
        static bool LoadFile(const std::string &path, BufferStream &buf);

    private:
        Texture2D *ParseTexture(const Identifier &ident, TEXTURE2D_DESC *pDesc, const std::string &path);

    private:
        std::unordered_map<Identifier, IResource<> *> m_Resources;
    };

}  // namespace Lorr