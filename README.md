# Ultra-fast Screen-Space Refractions and Caustics via Newton's Method
## Implementation of Newton's Method for rendering refractions and caustics
*Caustics are adapted from https://medium.com/@martinRenou/real-time-rendering-of-water-caustics-59cda1d74aa to use our novel application of Newton's method.*

*Ray marching (for comparison) implemented via https://jcgt.org/published/0003/04/04/ for pixel-perfect results.*

### Dependencies

- **raylib** — small OpenGL wrapper. Prebuilt .lib and headers are included in the repository.
- **Dear ImGui** — debug UI (added as a git submodule in `NewtonsMethodRefraction/vendor_imgui`, pinned to v1.92.4).
- **rlImGui** — raylib backend for ImGui (submodule in `NewtonsMethodRefraction/vendor_rlimgui`).

### Cloning

The repo uses git submodules for ImGui and rlImGui, so clone with `--recursive`:

```
git clone --recursive <repo-url>
```

If you already cloned without `--recursive`, initialize the submodules:

```
git submodule update --init --recursive
```

### Building

Open `NewtonsMethodRefraction.sln` in Visual Studio 2022 and build (Debug or Release, x64).

### Controls

- **WASD** — move along camera axes (W follows view direction, including pitch)
- **Mouse** — look
- **Space / Left Shift** — move up/down in world space
- **1 / 2** — snap to preset viewpoints
- **TAB** — toggle mouse capture (release to interact with the ImGui overlay)
- **T** — start/stop benchmarking the water shader (prints average frame time to stdout)
- **K** — save `screenshot.png` in the working directory
- **Esc** — quit

### ImGui perf overlay

The debug window in the top-left shows per-pass GPU timings (shadow, caustics, opaque, skybox+blit, water/refraction) using an async double-buffered `GL_TIMESTAMP` query, so it doesn't stall the CPU/GPU.

### Success-rate counters

Snippets of code can be commented/uncommented to record success and failure rates for either caustics or eye-ray refractions. Note that the water shader also contains screen-space reflections and other effects by default, bloating its execution time.