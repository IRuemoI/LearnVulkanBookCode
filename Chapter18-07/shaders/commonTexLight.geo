#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals { //���ͳ�����
layout(offset = 0) mat4 mvp; //�ܱ任����
layout(offset = 64) vec4 needsNormal; //�Ƿ���Ʒ������Ŀ��Ʊ�־
} myConstantVals;
layout (triangles) in; //����ͼԪΪ������
layout (line_strip) out; //���ͼԪΪ����
layout (max_vertices = 9) out; //������������Ϊ9
layout (location = 0) in vec3 inTeNormal[3]; //��ϸ����ֵ��ɫ�����ݹ����ķ�����(�任��)
layout (location = 1) in vec2 inVTexCoor[3]; //��ϸ����ֵ��ɫ�����ݹ�������������
layout (location = 2) in vec3 inVPosition[3]; //��ϸ����ֵ��ɫ�����ݹ�����λ������
layout (location = 3) in vec3 teNormalOri[3]; //��ϸ����ֵ��ɫ�����ݹ����ķ�����(ԭʼ��)
layout (location = 0) out vec3 teNormal; //�����ƬԪ��ɫ���ķ�����
layout (location = 1) out vec2 vTexCoor; //�����ƬԪ��ɫ������������
layout (location = 2) out vec3 vPosition; //�����ƬԪ��ɫ����λ������
layout (location = 3) out float fxlFlag; //�����ƬԪ��ɫ���ķ��������ƿ��Ʊ�־
in gl_PerVertex{ //����ӿڿ�
vec4 gl_Position; //����Ķ���λ������
} gl_in[];
out gl_PerVertex { //����ӿڿ�
vec4 gl_Position; //�����ƬԪ��ɫ���Ķ������λ��
};
void main(void){
int i; //ѭ�����Ƹ�������
for (i = 0; i < 3; i++){ //���������ε���������
gl_Position = myConstantVals.mvp*gl_in[i].gl_Position; //���������λ�ô��ݸ�����
teNormal=inTeNormal[i]; //�����������ݸ�ƬԪ��ɫ��
vTexCoor=inVTexCoor[i]; //���������괫�ݸ�ƬԪ��ɫ��
vPosition=inVPosition[i]; //������λ�ô��ݸ�ƬԪ��ɫ��
fxlFlag=0.0; //�����������ƿ��Ʊ�־���ݸ�ƬԪ��ɫ��
EmitVertex(); //����һ���������ݵ����
}
EndPrimitive(); //����һ��ͼԪ�����
if(myConstantVals.needsNormal[0]>0.5){ //����Ҫ���ƶ���ķ�����
for (i = 0; i < 3; i++) { //���������ε���������
gl_Position = myConstantVals.mvp*gl_in[i].gl_Position; //���㶥�����λ��(�������߶����)
teNormal=inTeNormal[i]; //�����������ݸ�ƬԪ��ɫ��
vTexCoor=inVTexCoor[i]; //���������괫�ݸ�ƬԪ��ɫ��
vPosition=inVPosition[i]; //������λ�ô��ݸ�ƬԪ��ɫ��
fxlFlag=1.0; //�����������ƿ��Ʊ�־���ݸ�ƬԪ��ɫ��
EmitVertex(); //����һ���������ݵ����
vec3 fxlEnd=gl_in[i].gl_Position.xyz+teNormalOri[i]*2.0; //�������߶��յ�λ��
gl_Position = myConstantVals.mvp*(vec4(fxlEnd,1)); //���㶥�����λ��(�������߶��յ�)
teNormal=inTeNormal[i]; //�����������ݸ�ƬԪ��ɫ��
vTexCoor=inVTexCoor[i]; //���������괫�ݸ�ƬԪ��ɫ��
vPosition=fxlEnd; //������λ�ô��ݸ�ƬԪ��ɫ��
fxlFlag=1.0; //�����������ƿ��Ʊ�־���ݸ�ƬԪ��ɫ��
EmitVertex(); //����һ���������ݵ����
EndPrimitive(); //����һ��ͼԪ�����
}}}