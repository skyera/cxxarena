# mycpp
c/c++/stl

### CodeChecker
- CodeChecker analyze compile_commands.json -o reports
- CodeChecker parse reports -e baseline -o reports.baseline
- CodeChecker analyze compile_commands.json -o reports-new
- CodeChecker cmd diff -b reports.baseline -n reports-new --new

