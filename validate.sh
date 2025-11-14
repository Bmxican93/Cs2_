#!/bin/bash
# Code Validation Script for CS2 Modern ImGui UI

echo "=== CS2 Modern ImGui UI - Code Validation ==="
echo ""

# Check for required files
echo "Checking file structure..."
required_files=(
    "main.cpp"
    "Gui/custom_widgets.h"
    "Gui/custom_widgets.cpp"
    "CMakeLists.txt"
    "README.md"
    ".gitignore"
)

missing_files=0
for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        echo "✓ $file exists"
    else
        echo "✗ $file missing"
        missing_files=$((missing_files + 1))
    fi
done

echo ""
echo "=== Security Checks ==="

# Check for unsafe functions
echo "Checking for unsafe string functions..."
unsafe_funcs=("strcpy" "sprintf" "gets" "strcat" "scanf")
found_unsafe=0

for func in "${unsafe_funcs[@]}"; do
    if grep -r "$func" main.cpp Gui/custom_widgets.cpp 2>/dev/null | grep -v "ImFormatString" | grep -v "snprintf"; then
        echo "✗ Found unsafe function: $func"
        found_unsafe=$((found_unsafe + 1))
    fi
done

if [ $found_unsafe -eq 0 ]; then
    echo "✓ No unsafe string functions found"
fi

# Check for buffer boundaries
echo ""
echo "Checking buffer usage..."
if grep -n "sizeof" main.cpp Gui/custom_widgets.cpp | grep -E "(snprintf|ImFormatString|InputText)"; then
    echo "✓ Found bounded buffer operations"
else
    echo "⚠ No explicit buffer bounds found"
fi

# Check for null pointer checks
echo ""
echo "Checking for window skip items..."
if grep -n "window->SkipItems" Gui/custom_widgets.cpp > /dev/null; then
    echo "✓ Widget functions check SkipItems"
else
    echo "⚠ Missing SkipItems checks in some widgets"
fi

# Check for memory leaks
echo ""
echo "Checking for resource cleanup..."
if grep -n "Release()" main.cpp > /dev/null; then
    echo "✓ Found DirectX resource cleanup code"
else
    echo "⚠ Missing resource cleanup"
fi

# Check for const correctness
echo ""
echo "Checking const usage..."
const_count=$(grep -c "const " main.cpp Gui/custom_widgets.cpp)
echo "✓ Found $const_count uses of const"

# Code metrics
echo ""
echo "=== Code Metrics ==="
echo "Lines of code:"
wc -l main.cpp Gui/custom_widgets.cpp Gui/custom_widgets.h

echo ""
echo "Function count in custom_widgets.cpp:"
grep "^.*::" Gui/custom_widgets.cpp | wc -l

echo ""
echo "=== Documentation Check ==="
docs=("README.md" "BUILD_GUIDE.md" "UI_DESIGN.md" "DESIGN_COMPARISON.md")
for doc in "${docs[@]}"; do
    if [ -f "$doc" ]; then
        lines=$(wc -l < "$doc")
        echo "✓ $doc exists ($lines lines)"
    else
        echo "✗ $doc missing"
    fi
done

echo ""
echo "=== Summary ==="
if [ $missing_files -eq 0 ] && [ $found_unsafe -eq 0 ]; then
    echo "✓ All checks passed!"
    exit 0
else
    echo "⚠ Some issues found"
    exit 1
fi
