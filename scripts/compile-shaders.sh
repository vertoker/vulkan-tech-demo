
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd "$SCRIPT_DIR/.."

INPUT="src/shaders"
OUTPUT="resources/ShaderCache"

mkdir "$OUTPUT"

glslc "$INPUT/world.vert" -o "$OUTPUT/world.vert.spv"
glslc "$INPUT/world.frag" -o "$OUTPUT/world.frag.spv"
glslc "$INPUT/point_light.vert" -o "$OUTPUT/point_light.vert.spv"
glslc "$INPUT/point_light.frag" -o "$OUTPUT/point_light.frag.spv"
