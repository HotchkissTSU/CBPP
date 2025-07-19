#version 400 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

in VS_OUT {
    vec2 aPos[2];
    vec2 aUV[2];
    float fAngle;
    float fDepth;
} gs_in[];

uniform float u_fScreenRatio;

/*
    3 ----- 4
    | \     |
    |   \   |
    |     \ |
    1 ----- 2
*/

out vec2 i_vUV;

vec2 RotateVector(in vec2 vSource, in float fAngle) {
    return vec2(
        vSource.x*cos(fAngle) - vSource.y*sin(fAngle),
        vSource.x*sin(fAngle) + vSource.y*cos(fAngle)
    );
}

void main() {
    vec2 vPos1 = gs_in[0].aPos[0];
    vec2 vPos2 = gs_in[0].aPos[1];

    vec2 vUV1 = gs_in[0].aUV[0]; // top-left
    vec2 vUV2 = gs_in[0].aUV[1]; // bottom-right

    vec2 aTexCoords[4];
    aTexCoords[0] = vUV1;
    aTexCoords[1] = vec2( vUV2.x, vUV1.y );
    aTexCoords[2] = vec2( vUV1.x, vUV2.y );
    aTexCoords[3] = vUV2;
    
    vec2 aVtxCoords[4];
    aVtxCoords[0] = vPos1;
    aVtxCoords[1] = vec2( vPos2.x, vPos1.y );
    aVtxCoords[2] = vec2( vPos1.x, vPos2.y );
    aVtxCoords[3] = vPos2;
    
    for(int i = 0; i < 4; i++) {
        vec2 vSource = aVtxCoords[i] - vPos1;
        vec2 vRot = vec2(
            vSource.x*cos(gs_in[0].fAngle) - vSource.y*sin(gs_in[0].fAngle),
            vSource.x*sin(gs_in[0].fAngle) + vSource.y*cos(gs_in[0].fAngle)
        );

        gl_Position = vec4((vRot + vPos1) / vec2(u_fScreenRatio, 1.0), gs_in[0].fDepth, 1.0);
        i_vUV = aTexCoords[i];

        EmitVertex();
    }
    EndPrimitive();
}
