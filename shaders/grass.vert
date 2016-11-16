#version 330

in vec4 point;
in vec2 position;
in vec2 variance;
in vec2 uvpoint;

uniform mat4 camera;

out vec2 UV;

vec4 bend(vec4 p)
{
    float magn = sqrt(variance[0]*variance[0]+variance[1]*variance[1]);
    mat4 rotationWind = mat4(1.0);
    rotationWind[0][0] = variance[0]/magn;
    rotationWind[2][0] = - variance[1]/magn;
    rotationWind[0][2] = - rotationWind[2][0];
    rotationWind[2][2] = rotationWind[0][0];

    float phi = magn;
    vec4 t = rotationWind * vec4(
        0,
        .5*p.y*(p.y*(cos(phi)-1)+2),
        .5*sin(phi)*p.y*p.y,
        1
    );
    t.x += p.x;
    return t;
}

void main()
{
    mat4 scaleMatrix = mat4(1.0);
    scaleMatrix[0][0] = 0.03;
    scaleMatrix[1][1] = 0.03;
    scaleMatrix[2][2] = 0.03;
    mat4 positionMatrix = mat4(1.0);
    positionMatrix[3][0] = position.x;
    positionMatrix[3][2] = position.y;

    UV = uvpoint;
	gl_Position = camera * (positionMatrix * scaleMatrix * bend(point));
}
