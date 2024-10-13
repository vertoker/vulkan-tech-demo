
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd "$SCRIPT_DIR/.."

INPUT="src/shaders"
OUTPUT="resources/ShaderCache"

mkdir "$OUTPUT"

glslc "$INPUT/simple.vert" -o "$OUTPUT/simple.vert.spv"
glslc "$INPUT/simple.frag" -o "$OUTPUT/simple.frag.spv"
