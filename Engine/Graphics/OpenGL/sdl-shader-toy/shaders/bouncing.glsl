#version 410 core

uniform vec2 iResolution;
uniform float iTime;

const vec2 eps = vec2(1e-4, 0.);

vec3 rotateZ(vec3 p, float a) 
{
	return vec3(cos(a)*p.xy+sin(a)*vec2(p.y,-p.x), p.z);
}

vec3 rotateY(vec3 p, float a) 
{
    vec2 u = cos(a)*p.xz+sin(a)*vec2(p.z,-p.x);
	return vec3(u.x, p.y, u.y);
}

float sdBox(vec3 p, vec3 b)
{	
    vec3 d = abs(p)-b;
    return min(max(d.x,max(d.y, d.z)),0.)+length(max(d,0.));
}

float sdSphere(vec3 p, vec3 c, float r)
{
    return length(p - c) - r;
}

vec2 bounce;

float map(vec3 p)
{
    p -= vec3(bounce, 0.);
    p = rotateZ(p, iTime);
    p = rotateY(p, iTime);
    return 
        max(
        	max(
            	max(
            		-sdBox(p + vec3(0., -0.3, 0.), vec3(2.0, 0.15, 2.0)),
        			-sdBox(p + vec3(0., +0.3, 0.), vec3(2.0, 0.15, 2.0))
        		),
        		max(
            		-sdBox(p + vec3(0., -0.9, 0.), vec3(2.0, 0.15, 2.0)),
        			-sdBox(p + vec3(0., +0.9, 0.), vec3(2.0, 0.15, 2.0))
        		)
        	),
            max(
        		-sdSphere(p, vec3(0., 0., 0.), 1.4),
            	sdSphere(p, vec3(0., 0., 0.), 1.5)
            )
    	);
}

vec3 normal(vec3 p)
{
    return normalize(vec3(
        map(p+eps.xyy)-map(p-eps.xyy),
		map(p+eps.yxy)-map(p-eps.yxy),
        map(p+eps.yyx)-map(p-eps.yyx)));
}

const vec3 light = vec3(1.0, 1.0, -2.0);
#define rgb(r,g,b) vec3(r,g,b)/256.

void mainImage(out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = (fragCoord-iResolution.xy*.5) / iResolution.x;

	// camera    
    vec3 org = vec3(0., 0., -5.0);  
    vec3 dir = normalize(vec3(uv, 1.0));

    vec3 col = rgb(55, 15, 18);
    
    bounce.x = abs(.5-fract(0.1*iTime))*5.-1.;
    bounce.y = abs(.5-fract(0.3*iTime))*5.-1.;
    
    // sphere tracing
    float d, r = 0.;
    for (int i = 0; i < 200; i++) {
        vec3 p = org + dir * d;
        r = map(p);
        if (d > 100.0)
            break;
        if (r < eps.x) {
            vec3 n = normal(p);
            vec3 pl = light-p;
        	float diffuse = max(0.0, dot(n, normalize(pl)));
            // TODO: blinn-phong
            col = vec3(.8, .6, .7) * diffuse + rgb(207, 163, 21);
            break;
        }
        d += r;
    }
    
    vec3 gamma = vec3(1.0/2.2);
    fragColor = vec4(pow(col, gamma), 1.0);
}

void main() 
{
    vec4 fragColor;
    mainImage(fragColor, gl_FragCoord.xy);
    gl_FragColor = fragColor;
}
