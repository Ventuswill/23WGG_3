$input a_position, a_texcoord0
$output v_texcoord0

#include <bgfx_shader.sh>

uniform vec4 u_Scale;
uniform vec4 u_Rotation;
uniform vec4 u_Position;
uniform vec4 u_CameraPosition;
uniform vec4 u_ProjectionScale;
uniform vec4 u_UVScale;
uniform vec4 u_UVOffset;

vec2 TransformIntoWorldSpace(vec2 pos);

void main()
{
	vec2 objectSpacePosition = a_position;
	vec2 worldSpacePosition = TransformIntoWorldSpace( objectSpacePosition );
	vec2 viewSpacePosition = worldSpacePosition - u_CameraPosition.xy;
	vec2 clipSpacePosition = viewSpacePosition * u_ProjectionScale.xy;
	gl_Position = vec4( clipSpacePosition, 0, 1 );

	v_texcoord0 = a_texcoord0 * u_UVScale.xy + u_UVOffset.xy;
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
