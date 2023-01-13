$input a_position

#include <bgfx_shader.sh>

uniform vec4 u_Scale;
uniform vec4 u_Rotation;
uniform vec4 u_Position;
uniform vec4 u_CameraPosition;
uniform vec4 u_ProjectionScale;

vec2 TransformIntoWorldSpace(vec2 pos);

void main()
{
	vec2 objectSpacePosition = a_position;
	vec2 worldSpacePosition = TransformIntoWorldSpace( objectSpacePosition );
	vec2 viewSpacePosition = worldSpacePosition - u_CameraPosition.xy;
	vec2 clipSpacePosition = viewSpacePosition * u_ProjectionScale.xy;
	gl_Position = vec4( clipSpacePosition, 0, 1 );
}

vec2 TransformIntoWorldSpace(vec2 pos)
{
	// Scale.
	pos *= u_Scale.xy;
	
	// Rotate.
	float newX = cos(u_Rotation.x)*pos.x + -sin(u_Rotation.x)*pos.y;
	float newY = sin(u_Rotation.x)*pos.x + cos(u_Rotation.x)*pos.y;
	pos = vec2(newX, newY);
	
	// Translate.
	pos += u_Position.xy;

	return pos;
}
