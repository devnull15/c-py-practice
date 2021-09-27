#!/usr/bin/env python3
import pathlib
import requests
import hashlib
import yaml
from junit_xml import TestSuite, TestCase

ORACLE_HOSTNAME = "oracle"

TEST_SUITE_OUTPUT = "suite_results.xml"

RESPONSE_EXTENSION = ".response.yaml"

GRADED_PHRASE = { False : "Incorrect" , True : "Correct" }

SHOW_ALL = False

ts = []

def ask_oracle(jqr_item,question):
    question_hash = hashlib.md5(question["text"].encode("utf-8") + question["type"].encode("utf-8")).hexdigest()
    student_submission = { "submission" : question["resp"] }
    resp = requests.post("http://{}/{}/{}".format(ORACLE_HOSTNAME,jqr_item,question_hash),data = None, json=student_submission)
    return resp.text == "True"

for f in pathlib.Path('.').iterdir():
    if f.is_file() and RESPONSE_EXTENSION == f.name[-len(RESPONSE_EXTENSION):]:
        jqr_original_name = pathlib.Path(f.name[:-len(RESPONSE_EXTENSION)])
        jqr_item_obj_text = f.read_text()
        jqr_item_obj = yaml.safe_load(jqr_item_obj_text)
        test_cases = []
        index = 0
        for question in jqr_item_obj:
            result = question
            result["pass"] = ask_oracle(jqr_original_name,question)
            stderr = GRADED_PHRASE[result["pass"]]
            cur_test_case = TestCase("{}-{}".format(f.name,index),"test.class",0.0,"{}-{}".format(result["text"],result["type"]),stderr)
            if not result["pass"] or SHOW_ALL:
                print("In file {}, on question ({}): {}\n\tYour Response: {}\n\t\t\t{}\n".format(jqr_original_name,result["type"],result["text"],result["resp"],GRADED_PHRASE[result["pass"]]))
                cur_test_case.add_failure_info(message=stderr)
            test_cases.append(cur_test_case)
        cur_test_suite = TestSuite(jqr_original_name,test_cases)
        ts.append(cur_test_suite)
        index +=1
if len(ts) > 0:
    with open(TEST_SUITE_OUTPUT, 'w') as f:
        TestSuite.to_file(f, ts, prettyprint=True)
