#version 330 core
// The first line of any shader must a '#version" command which defines the glsl version to use

// This will be the output (you can choose any name) and it will be sent to the frame buffer (back buffer of the window).
out vec4 frag_color;
in vec4 gl_FragCoord ;
vec2 iResolution=vec2(1280,720);
uniform int keyboard;
uniform vec2 mouse;
// This is entry point of the fragment shader and it will be called for every fragment covered by the rasterized geometry
void drawCircle (vec2 pos, float diameter, vec4 color1)
    {   
        pos.x*=iResolution.x/iResolution.y;// normalizing
        float dist = length(pos);
    
        if(dist < diameter){
        frag_color=color1;
        }
        
    }
vec2 calcCenter(float x , float y, vec2 screenPos  )
    {
        screenPos.x -= x;
        screenPos.y -= y;
        return screenPos;
    }
 void drawHeart(vec2 screenPos)
    {
        // Draw Circle 1
        screenPos=calcCenter(0.5,0.5,screenPos);
        screenPos.y-=abs(screenPos.x);
        drawCircle(screenPos,0.2,vec4(1.0,1.0,1.0,1.0));

    }


    #define PI 3.14159265359
#define TWO_PI 6.28318530718

void drawGoogle( vec2 screenPos )
{
    
        // Draw Circle 1
        screenPos=calcCenter(0.5,0.5,screenPos);
        drawCircle(screenPos,0.2,vec4(1.0,1.0,1.0,1.0));

        // Draw Circle 2
        drawCircle(screenPos,0.12,vec4(0.0,0.0,0.0,1.0));

        float m=1.5;
        float m1=-1.5;
        if ((screenPos.x*m) >screenPos.y && (screenPos.x*m1) <screenPos.y)
            frag_color =vec4(0.0,0.0,0.0,0.0);  

        screenPos=calcCenter(-0.5,-0.5,screenPos);

         if(screenPos.x > 0.54 && screenPos.x < 0.586 && screenPos.y > 0.372 && screenPos.y < 0.5)
          frag_color = vec4(1.0, 1.0, 1.0, 1.0); 
   
   
}
void main() {
    
    // Here we just output a constant color which is red (R=1, G=0, B=0, A=1)
  
    frag_color=vec4(0.0,0.0,0.0,1.0);

    
    vec2 screenPos = (gl_FragCoord.xy/iResolution.xy);
    screenPos.x -= mouse.x/iResolution.x-0.5;
    screenPos.y += mouse.y/iResolution.y-0.5;
    if (keyboard==1)
        drawHeart(screenPos);
    else if (keyboard==2)
        drawGoogle(screenPos);
    //drawHeart(screenPos);
  
}