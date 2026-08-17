<p align="center">
  <img src="https://github.com/alxmra/gpu-mandelbrot/blob/main/images/one.gif" alt="Mandelbrot zoom demo" width="600">
</p>
<h1 align="center"><a href="https://en.wikipedia.org/wiki/Mandelbrot_set">Mandelbrot Set</a> on OpenGL</h1>

A short learning experiment on [Shader](https://en.wikipedia.org/wiki/Shader) programming using C++ and OpenGL.

<p align="center">
  <img src="https://github.com/alxmra/gpu-mandelbrot/blob/main/images/two.gif" alt="Mandelbrot zoom demo" width="600">
</p>

## Build and run

In order to run this piece of art, one must have GLFW, GLEW GLM and a GPU:

```bash
make
./mandelbrot
```

## Controls

| Key            | Action              |
|----------------|----------------------|
| Arrow keys     | Pan                  |
| Left Shift     | Zoom in              |
| Left Ctrl      | Zoom out             |
| `+` (`[` for US keyboards)  | Morph shape (+)*     |
| `-` (`/`)                   | Morph shape (-)*     |
| Esc / Q        | Quit                 |

* Note: Morphing shifts the Real part of the computation merely for the sake of trying it out in real time.
