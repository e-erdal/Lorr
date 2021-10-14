# v1.2-alpha
- OOP Entity Component System.
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