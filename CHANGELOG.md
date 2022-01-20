# v1.5-alpha
- Code:
    - Rename interface classes to `BaseMyClass` (from `IMyClass`)
    - Rename global enums to enum classes.
    - Hide (some) external library implementations to speedup compile time.
    - Move to EASTL.
    - Remove FMT.
    - Remove spdlog, use loguru.
- Graphics:
    - Redesign for Renderer, it's now less abstracted from API functions.
    - (NOT COMPLETE) Prototype Terrain generation.
        - Able to use custom Vertex Data types without big changes.
        - Can be split into multiple chunks.
        - Has support for Perlin Noise.
    - Move `SwapChain` implementation to seperate class.
    - Add support for more texture formats.
- Scripting:
    - Add lua scripting.
- Resources & Resource Compiler:
    - Less messy way of compiling resources.
    - Categorize resource loading using a global metafile.
- Vendor:
    - Categorized External Library to get less confused.

# v1.2-alpha
- Entity Component System.
    - Transform component.
    - Renderable component.
- Font rendering.
    - Basic UTF8 implementation.
- Native renderer abstraction.
    - Cross-API shader input layout.
    - Support for D3D11.
        - Able to create D3D11Texture2D.
        - Able to create D3D11VertexShader.
        - Able to create D3D11PixelShader.
        - Vertex shader D3D11InputLayout.
    - Base for D3D12.
- BufferStream memory watcher.

# v1.1-alpha
- Audio system.
    - Able to decode Wav.
    - Able to decode Ogg.
    - Able to decode MP3.
- Rework on ResourceMan.
- Batched rendering system.
- Console Application mode(no rendering).
- ImGui rendering.

# v1.0-alpha
- Win32 window handling.
- ImGui context.
- Cross platform rendering(BGFX).
- File and buffer stream.
- Cool logger.