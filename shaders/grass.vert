#version 330

in vec4 point;
in vec2 position;
in vec4 variance;
in vec2 uvpoint;

uniform mat4 camera;

out vec2 UV;

void main() {
    mat4 scaleMatrix = mat4(1.0);
    scaleMatrix[0][0] = 0.03;
    scaleMatrix[1][1] = 0.03;
    scaleMatrix[2][2] = 0.03;
    mat4 positionMatrix = mat4(1.0);
    positionMatrix[3][0] = position.x;
    positionMatrix[3][2] = position.y;

    float t = point.y;
    float phi = 10*variance[0];
    float l = 1;

    vec4 p = vec4(
        point.x,
        .5*l*t*(t*(cos(phi)-1)+2),
        .5*l*sin(phi)*t*t,
        1
    );

    UV = uvpoint;
	gl_Position = camera * (positionMatrix * scaleMatrix * p);
	    //+ variance * point.y);
}
