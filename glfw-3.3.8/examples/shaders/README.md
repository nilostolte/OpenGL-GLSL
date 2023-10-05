# Shaders

These examples illustrate several shaders variants for displaying 2D graphics using OpenGL.

## Implicit circle with adaptive antialiasing

The programs [circle.c](https://github.com/nilostolte/OpenGL-GLSL/blob/main/glfw-3.3.8/examples/shaders/circle.c) and
[circle_bleding.c](https://github.com/nilostolte/OpenGL-GLSL/blob/main/glfw-3.3.8/examples/shaders/circle_bleding.c), both display
implicit circles with a custom adaptive antialising. The only difference is that
[circle_bleding.c](https://github.com/nilostolte/OpenGL-GLSL/blob/main/glfw-3.3.8/examples/shaders/circle_bleding.c) uses
alpha blending, thus allowing blending with previous displayed objects or with the background color.

To implement the alpha blending, the function `circle`, instead of returning a color, only returns the correspondent
transparency (alpha value - see code below), which is then pegged to the color of the object. If the alpha value is 
zero the content of the fragment is ignored. If it's between zero and one, the color of the object is blended with 
what was previously displayed. If it's one, the color of the object overwrites whatever has been displayed previously. 
This approach is betterthan used in 
[circle.c](https://github.com/nilostolte/OpenGL-GLSL/blob/main/glfw-3.3.8/examples/shaders/circle.c). However,
it does not work on Shadertoy.

```GLSL
float circle(vec2 p, vec2 c, float r, float w) {
    float x = p.x - c.x;
    float y = p.y - c.y;
    float d = x*x + y*y;
    float r2 = r*r;
    if (d > r2) {
      if ( (d-r2) < (w*w) ) {
        float n = sqrt(d-r2);
        float v = smoothstep(0.,w, n);
        return (1.-v);
      }
      return 0.;
    }
    return 1.;
}
```

### Adaptive antialiasing

As observed in the code above, if the distance of a point to the circle center is greater than the circle radius 
but the difference between this distance and the radius is smaller than the width `w`, the antialising takes place.

However, for a proper antialiazing the width `w` should vary for different screen resolutions. The problem is that,
when one changes the size of the window the graphics is scaled, but when it's scaled up the antialising region thickness
might be too large and noticiable. When one sets `w` for full screen, the antialiasing will not be enough in other window 
sizes and the object will appear jaggy.

This clearly shows that `w` should be different for every different window size. The reason for that is that lower 
resolutions require proportionally larger values of `w`, because the image is more jaggy than in higher resolutions. 
The scale alone does not solve the problem, since in higher resolutions `w` should be indeed numerically larger than 
in lower resolutions, but not as much as the automatic scale adjusting the size of the object.

What one needs is a linear function that will give the values of `w` based on two values visually adapted for 
two diferent and far apart resolutions.

This is shown in the function `mainImage` in the fragment shader:

```GLSL
void mainImage( out vec4 fragColor, in vec2 fragCoord ){
    vec2 uv = fragCoord/iResolution.y;
    float iy = 1.0/iResolution.y;
    float ar = iResolution.x * iy;
    vec3 obj = vec3(0.2, 0.5, 1.);  
    vec2 c = vec2(0.5*ar, 0.5); 
    float minr = 236.;
    float maxr = 768.;
    float maxpx = 26.;
    float minpx = 14.;
    float a = (maxpx - minpx) / (maxr - minr);
    float b = minpx - (a * minr);
    fragColor = vec4(obj,circle(uv, c, 0.35, a + iy*b));
}
void main() {
    mainImage(gl_FragColor, gl_FragCoord.xy);
};
```
One visually determines that for the resolution 236, one will need 14 pixels, and for the resolution 768 one will
need 26 pixels. Notice that the chosen ratio is less than 2, instead of 3.2 obtained by scaling from one resolution
to another. One then calculates $a$ and $b$, respectively the slope and the $y$ value when 
$x = 0$. In the equation $y = ax + b$, $x$ corresponds to the resolution
(not the axis x) and $y$ corresponds to the number of pixels calculated for the antialiasing.

As the code indicates, the scale adjustment of the coordinates is done by dividing both coordinates by the 
resolution component corresponding to the y axis ($iResolution.y$ or $x$). This is done to avoid distortion in the image, otherwise 
the circle would be displayed as an ellipse. Thus, the pixel size ($iy$) becomes:

![](https://latex.codecogs.com/svg.image?iy=\frac{1}{iResolution.y}=\frac{1}{x})

But what one needs is the real width of these pixels. Therefore, one needs to multiply the size of the pixel
to the value obtained. Since the size of the pixel actually corresponds to $1/x$,

![](https://latex.codecogs.com/svg.image?w=ax\times\frac{1}{x}&plus;b\times\frac{1}{x}=a&plus;b\times\frac{1}{x}=a&plus;b\times&space;iy)

Which is passed to the function cricle in the code.

### Circle with alpha blending

High resolution:

<img src="https://github.com/nilostolte/OpenGL-GLSL/assets/80269251/446cb3bd-5628-4b48-965f-8a81def8d29d" width=512>

Detail of the antialising:

<img src="https://github.com/nilostolte/OpenGL-GLSL/assets/80269251/fc86145d-cc4d-443a-bf56-bce04a8144dc" width=512>

Low resolution:

![image](https://github.com/nilostolte/OpenGL-GLSL/assets/80269251/1c27c57d-6b4a-485c-bebf-75caca6804bc)

Detail of the antialiasing:

<img src="https://github.com/nilostolte/OpenGL-GLSL/assets/80269251/e57a6cab-663f-447e-8d97-e80727a4c3ac" width=512>


As can be seen, in practice, the real number of pixels in the antialiasing zone does not correspond to the numbers 
used in the program. They can be seen more as numerical parameters. In any case, they were dertermined by the 
visual effect obtained.

