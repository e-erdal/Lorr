//
// Created on Monday 15th November 2021 by e-erdal
//
//? ECS-like design choice. Wanted to make this as much modular as possible.
//? Pass/phase order will depend on how you register it, first is always first.
//? Manager will be seperated from Engine/Managers folder.
//? PostProcessMan has no connection but LorrGraphics module.
//

#pragma once

namespace lr
{
    // Yet another interface
    class PostProcessManager;
    class IPostProcess
    {
    public:
        IPostProcess() = default;

        // Function to init shaders/buffers, etc...
        virtual void Init() = 0;
        virtual void Draw() = 0;

    protected:
        eastl::string_view m_Name;

        friend PostProcessManager;
    };

    class PostProcessManager
    {
    public:
        PostProcessManager() = default;

        void Init();
        void Draw();

        template<typename T = IPostProcess>
        void Register()
        {
            T *pPass = new T;
            pPass->Init();

            m_Passes[typeid(T).hash_code()] = pPass;
        }

        template<typename T = IPostProcess>
        T *Get()
        {
            for (auto pass : m_Passes)
                if (pass.first == typeid(T).hash_code()) return pass.second;

            return 0;
        }

    private:
        eastl::unordered_map<size_t, IPostProcess *> m_Passes;
    };

}  // namespace lr