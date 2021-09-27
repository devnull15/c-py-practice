#!/usr/bin/env python3
import pathlib

from quiz import *
from junit_xml import TestSuite, TestCase


TEST_SUITE_OUTPUT = "suite_results.xml"

RESPONSE_EXTENSION = ".response.yaml"

GRADED_PHRASE = { False : "Incorrect" , True : "Correct" }

SHOW_ALL = False

ts = []
for f in pathlib.Path('.').iterdir():
    if f.is_file() and RESPONSE_EXTENSION == f.name[-len(RESPONSE_EXTENSION):]:
        source_file = pathlib.Path(f.name[:-len(RESPONSE_EXTENSION)])
        if source_file.is_file():
            cur_quiz = quiz_builder(f.name,out_gradefile=f.name+".grade")
            cur_quiz.import_solution_from_file(source_file.name)
            cur_quiz.grade_quiz()
            test_cases = []
            index = 0
            for result in cur_quiz.export_test(display=False):
                stderr = GRADED_PHRASE[result["pass"]]
                cur_test_case = TestCase("{}-{}".format(f.name,index),"test.class",0.0,"{}-{}".format(result["text"],result["type"]),stderr)
                if not result["pass"] or SHOW_ALL:
                    print("In file {}, in question ({}): {}\n\tYour Response: {}\n\t\t\t{}\n".format(source_file.name,result["type"],result["text"],result["resp"],GRADED_PHRASE[result["pass"]]))
                    cur_test_case.add_failure_info(message=stderr)
                test_cases.append(cur_test_case)
            cur_test_suite = TestSuite(source_file.name,test_cases)
            ts.append(cur_test_suite)
            index +=1
if len(ts) > 0:
    with open(TEST_SUITE_OUTPUT, 'w') as f:
        TestSuite.to_file(f, ts, prettyprint=True)
