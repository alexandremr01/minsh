./bin/minsh < test/test-1.in > /tmp/null
if ! cmp -s test/test-1.expected.txt test/test-1.output.txt; then
  echo "Output does not match expected"
  exit 1
fi