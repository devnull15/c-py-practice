#!/usr/bin/env python3
import yaml
import pathlib
from sys import exit
from mako import exceptions
from mako.template import Template

JQR_SOURCE = "basic_ccd_jqr_module_3.yaml"

JQR_OUT_TEMPLATE = "jqr.template.adoc"
JQR_OUT_FN = "jqr.adoc"

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
f = open(JQR_SOURCE,"r")
jqr_yaml_source = f.read()
f.close()

jqr_dict = yaml.safe_load(jqr_yaml_source)

jqr_processed = {}

for top_item in jqr_dict.keys():
    cur_top_item = jqr_dict[top_item]
    jqr_processed[top_item] = {}
    jqr_processed[top_item]["topic" ] = cur_top_item["topic"]
    jqr_processed[top_item]["items"] = {}
    for item in cur_top_item["items"].keys():
        cur_item = cur_top_item["items"][item]
        jqr_processed[top_item]["items"][item] = {}
        jqr_processed[top_item]["items"][item]["title"] = cur_item["title"]
        jqr_processed[top_item]["items"][item]["condition"] = cur_item["condition"]
        jqr_processed[top_item]["items"][item]["standard"] = cur_item["standard"]
        jqr_processed[top_item]["items"][item]["complete"] = True
        jqr_processed[top_item]["items"][item]["sub_items"] = {}

        for sub_item in cur_item["sub_items"].keys():
            cur_sub = cur_item["sub_items"][sub_item]
            jqr_processed[top_item]["items"][item]["sub_items"][sub_item] = {}
            jqr_processed[top_item]["items"][item]["sub_items"][sub_item]["title"] = cur_sub
            jqr_processed[top_item]["items"][item]["sub_items"][sub_item]["complete"] = True
jqr_processed = {"jqr_items" : jqr_processed}
write_template(JQR_OUT_TEMPLATE, jqr_processed,JQR_OUT_FN)