# Usage guide

## How to run ipv4_cutter
- compile `gcc ipv4_cutter.c -o ipv4_cutter`
- run `./ipv4_cutter < INPUT_FILE > OUTPUT_FILE`

## How to test ipv4_cutter produced solution
- compile `gcc solution_tester.c -o solution_tester`
- run `./solution_tester INPUT_FILE ANSWER_FILE`

_This takes a while._<br>
_I would approximate time in seconds to_ <br>
`4 * MAX(ANS_SUBNETS_COUNT, INPUT_SUBNETS_COUNT)`<br>
_Which in input1 case is around 220 seconds or 4 minutes_.

The actual runtime complexity of the test is<br>
`O(2^32 * MAX(ANS_SUBNETS_COUNT, INPUT_SUBNETS_COUNT))`

