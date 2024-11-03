
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd "$SCRIPT_DIR/.."

INPUT="src/shaders"
OUTPUT="resources/ShaderCache"

mkdir "$OUTPUT"

glslc "$INPUT/world.vert" -o "$OUTPUT/world.vert.spv"
glslc "$INPUT/world.frag" -o "$OUTPUT/world.frag.spv"
