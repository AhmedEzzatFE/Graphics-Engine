#version 330 core
// Here we will define Uniform Variable
// they are called uniform because every run of the shader (under the same draw call) will see the same value
// so it is uniform across all executions of the shader
uniform vec3 color;
uniform float time;
uniform bool flicker;
uniform vec2 mouse;
uniform vec2 iResolution;
uniform int shape;

// The output that goes to the frame buffer
out vec4 frag_color;

float Circle (vec2 uv, vec2 p, float r, float blur) {
    float c = smoothstep(r, r-blur , length(uv-p));
    return c;
}

float Smiley(vec2 uv, vec2 p, float size) {

    uv -= p;

    float mask = Circle(uv, vec2(0.), .4, .01);

    mask -= Circle(uv, vec2(-.13, .2), .06, .01);
    mask -= Circle(uv, vec2(.13, .2), .06, .01);

    float mouth = Circle(uv, vec2(0., 0.), .3, .01);
    mouth -= Circle(uv, vec2(0., 0.1), .3, .01);

    mask -= mouth;

    return mask;
}


// Just a constant
#define PI 3.1415926535897932384626433832795
void main() {
    if(shape == 1) {
        vec2 uv= (gl_FragCoord.xy-iResolution.xy*.5)/iResolution.y;//(mouse/255)*

        uv -= .5;
        uv.x *= iResolution.x/iResolution.y;
        uv.x -= (mouse.x/iResolution.x-1.5f);
        uv.y += (mouse.y/iResolution.y);
        vec3 col = vec3(0.);

        float mask = Smiley(uv, vec2(0.), 1.);


        col = vec3(1., 1., 1.)* mask;
        frag_color = vec4(col, 1.0);
    }
    else if (shape == 2){
        frag_color = vec4(color, 1.0);
        // If flickering, multiply it with a sinusoidal wave that oscillates over time
        vec2 uv= (gl_FragCoord.xy-iResolution.xy*.5)/iResolution.y;//(mouse/255)*
        uv.x -= (mouse.x/iResolution.x-0.6f);
        uv.y += (mouse.y/iResolution.y-0.4f);
        vec3 col= vec3(0);
        uv.x *=.7;
        uv.y -=sqrt(abs(uv.x))*.6;
        float r=.3;
        float b=.01;
        float d = length(uv);
        float c= smoothstep(r,r-b,d);
        col= vec3(c);
        frag_color=vec4(col,1.0);
    }
    // Convert RGB to RGBA (in other words, add an alpha value).


}
