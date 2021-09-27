#!/usr/bin/env python3
import yaml
import pathlib
import xmltodict
import json
from sys import exit
from mako import exceptions
from mako.template import Template

JQR_SOURCE = "export/basic_ccd_jqr_module_3.yaml"

JQR_OUT_TEMPLATE = "export/jqr.template.adoc"
JQR_OUT_FN = "jqr.adoc"

EXERCISES_DIRECTORY = "exercises"
EXERCISE_METADATA_FILE = ".junit-expected.yaml"
EXERCISE_TYPES = [ "C", "Python", "Knowledge", "Special", "BSLE" ] 

DEBUG_VERBOSE = 2
DEBUG_NONE = 0
VERBOSITY = 2

def dbg(*msg, verbosity=DEBUG_VERBOSE):
        if verbosity <= VERBOSITY:
                print(*msg)

def err(*msg):
        dbg(*msg, verbosity=DEBUG_NONE)
        exit(1)

def error_to_template():
        error = "_error.html"
        f = open(error,"wb")
        f.write(exceptions.html_error_template().render())
        f.close()
        print(exceptions.text_error_template().render())
        err("Failed to render. Error also saved to: {}".format(error))


def write_template(fn,d,out):
        try:
                template_fn = str(fn)
                template = Template(filename=template_fn, input_encoding='utf-8', output_encoding='utf-8', encoding_errors='replace')
                rendered = template.render(**d)
                adoc_outfile = pathlib.Path(out)
                adoc_outfile.write_bytes(rendered)
        except:
                error_to_template()
doc = False
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

"""
>>> doc["testsuites"]["testsuite"][0].keys()
odict_keys(['@disabled', '@errors', '@failures', '@name', '@skipped', '@tests', '@time', 'testcase'])
"""

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
                                print(test_file_path)
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
f = open(JQR_SOURCE,"r")
jqr_yaml_source = f.read()
f.close()

jqr_dict = yaml.safe_load(jqr_yaml_source)



def cross_ref_jqr(jqr_dict,test_dict):
        jqr_processed = {}
        for top_item in jqr_dict.keys():
                cur_top_item = jqr_dict[top_item]
                jqr_processed[top_item] = {}
                jqr_processed[top_item]["topic" ] = cur_top_item["topic"]
                jqr_processed[top_item]["items"] = {}
                for item in cur_top_item["items"].keys():
                        cur_item = cur_top_item["items"][item]
                        cur_item_dependencies = cur_item.get("dependencies",[item])
                        jqr_processed[top_item]["items"][item] = {}
                        jqr_processed[top_item]["items"][item]["title"] = cur_item["title"]
                        jqr_processed[top_item]["items"][item]["condition"] = cur_item["condition"]
                        jqr_processed[top_item]["items"][item]["standard"] = cur_item["standard"]
                        jqr_processed[top_item]["items"][item]["complete"] = False
                        dependency_complete = {}
                        for item_dependency in cur_item_dependencies:
                                dependency_complete[item_dependency] = False
                                if item_dependency in test_dict.keys():
                                        if test_dict[item_dependency]["pass"]:
                                                dependency_complete[item_dependency] = True
                                jqr_processed[top_item]["items"][item]["sub_items"] = {}

                                for sub_item in cur_item["sub_items"].keys():
                                        cur_sub = cur_item["sub_items"][sub_item]
                                        jqr_processed[top_item]["items"][item]["sub_items"][sub_item] = {}
                                        jqr_processed[top_item]["items"][item]["sub_items"][sub_item]["title"] = cur_sub
                                        jqr_processed[top_item]["items"][item]["sub_items"][sub_item]["complete"] = jqr_processed[top_item]["items"][item]["complete"]
                                        if item_dependency in test_dict.keys():
                                                for test_item in test_dict[item_dependency]["sub_items"]:
                                                        if test_item["name"] == sub_item:
                                                                jqr_processed[top_item]["items"][item]["sub_items"][sub_item]["complete"] = test_item["pass"]
                        if len(dependency_complete.keys()) > 0:
                                overall_item_complete = True
                                for key_name in dependency_complete.keys():
                                        overall_item_complete = overall_item_complete and dependency_complete[key_name]
                                jqr_processed[top_item]["items"][item]["complete"] = overall_item_complete
                                if jqr_processed[top_item]["items"][item]["complete"]:
                                        for sub_item_key in jqr_processed[top_item]["items"][item]["sub_items"].keys():
                                                jqr_processed[top_item]["items"][item]["sub_items"][sub_item_key]["complete"] = jqr_processed[top_item]["items"][item]["complete"]
                        
        return {"jqr_items" : jqr_processed}
        


compiled_jqr = {}

exercises_dir = pathlib.Path(EXERCISES_DIRECTORY)

if not exercises_dir.is_dir():
        dbg("Cannot find directory: {}".format(EXERCISES_DIRECTORY))
else:
    for exercise_type in exercises_dir.iterdir():
            if exercise_type.is_dir() and exercise_type.name in EXERCISE_TYPES:
                    for exercise in exercise_type.iterdir():
                        if exercise.is_dir(): 
                                    junit_file = exercise / EXERCISE_METADATA_FILE
                                    if junit_file.is_file():
                                            compiled_jqr.update(parse_junit_meta_file(junit_file))
print("==Showing compiled Test Data===")
print(json.dumps(compiled_jqr,indent=4, sort_keys=True))
cross_referenced_jqr = cross_ref_jqr(jqr_dict,compiled_jqr)
print("==Showing Test Data after Cross-Reference with JQR===")
print(json.dumps(cross_referenced_jqr,indent=4, sort_keys=True))

write_template(JQR_OUT_TEMPLATE, cross_referenced_jqr,JQR_OUT_FN)
#write_template(JQR_OUT_TEMPLATE, jqr_processed,JQR_OUT_FN)


# list exercise directories
# per each, exercise, load the metadata file
# load the metadata file and as defined, load the junit XML
# cross reference everything to build the final report. 
