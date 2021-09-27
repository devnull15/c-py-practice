import yaml
import pathlib
import xmltodict
from sys import exit

JUNIT_FILE = ".junit-expected.yaml"


def parse_junit_file(junit_file):
        results = {}
        global doc
        if junit_file.is_file():
                doc = xmltodict.parse(junit_file.read_text())
                results["total_tests"] = doc["testsuites"]["@tests"]
                results["total_errors"] = doc["testsuites"]["@errors"]
                results["total_failures"] = doc["testsuites"]["@failures"]
                testsuites = doc["testsuites"]["testsuite"]
                if not isinstance(testsuites,list):
                        testsuites = [testsuites]
                for testsuite in testsuites:
                        results[testsuite["@name"]] = {  "errors" : testsuite["@errors"], "failures" : testsuite["@failures"], "total": testsuite["@tests"]}
        return results


def parse_junit_meta_file(junit_meta_file):
        junit_meta_obj = yaml.safe_load(junit_meta_file.read_bytes())
        results = {}
        for item in junit_meta_obj:
                jqr_item_key = item["overall"]
                results[jqr_item_key] = {}
                results[jqr_item_key]["pass"] = True
                results[jqr_item_key]["test_suites"] = {}
                results[jqr_item_key]["sub_items"] = []
                if "test_files" in item.keys():
                        for test_file in item["test_files"]:
                                test_file_path = junit_meta_file.parents[0] / test_file
                                if test_file_path.is_file():
                                        results[jqr_item_key]["test_suites"].update(parse_junit_file(test_file_path))
                if "sub_items" in item.keys():
                        for sub_item in item["sub_items"]:

                                test_suite = sub_item["name"]
                                expected_passes = int(sub_item["test_count"])
                                cur_sub = {"name" : test_suite, "expected_tests" : expected_passes}
                                cur_result = False
                                if test_suite in results[jqr_item_key]["test_suites"].keys():
                                        suite_result = results[jqr_item_key]["test_suites"][test_suite]
                                        cur_sub["fails"] = int(suite_result["failures"])
                                        cur_sub["errors"] = int(suite_result["errors"])
                                        cur_sub["actual_tests"] = int(suite_result["total"])
                                        if cur_sub["actual_tests"] == expected_passes and cur_sub["fails"] == 0 and cur_sub["errors"] == 0:
                                                cur_result = True

                                cur_sub["pass"] = cur_result
                                results[jqr_item_key]["sub_items"].append(cur_sub)
                                if not cur_result:
                                        results[jqr_item_key]["pass"] = False
                elif "total_tests" in results.keys() and "total_errors" in results.keys() and "total_failures" in results.keys():
                        if int(results["total_errors"]) > 0 or int(results["total_failures"]) > 0  or int(results["total_tests"]) == 0:
                                 results[jqr_item_key]["pass"] = False


        return results

parse_results = parse_junit_meta_file(pathlib.Path(JUNIT_FILE))
for item_key in parse_results.keys():
    if not parse_results[item_key]["pass"]:
       print("Did not pass expected test suites");
       print(parse_results)
       exit(1)
