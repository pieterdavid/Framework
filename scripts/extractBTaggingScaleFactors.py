#! /bin/env python

import math
import argparse
import csv
import sys

def operating_point_to_string(operating_point):
    if operating_point == 0:
        return "loose"
    elif operating_point == 1:
        return "medium"
    elif operating_point == 2:
        return "tight"
    elif operating_point == 3:
        return "discr_reshaping"
    else:
        return "unknown"

def jet_flavor_to_string(jet_flavor):
    if jet_flavor == 0:
        return "bjets"
    elif jet_flavor == 1:
        return "cjets"
    elif jet_flavor == 2:
        return "lightjets"
    else:
        return "unknown"
           

parser = argparse.ArgumentParser()
parser.add_argument('file', help='CSV file containing b-tagging scale factors')
parser.add_argument('-s', '--suffix', help='Suffix to append at the end of the output filename', required=True)

args = parser.parse_args()

# See https://twiki.cern.ch/twiki/bin/view/CMS/BTagCalibration for CSV format definition

all_json_content = {}

def get_token(operating_point, measurement_type, jet_flavor):
    return (operating_point, measurement_type, jet_flavor)

with open(args.file, 'r') as f:
    data = csv.reader(f, skipinitialspace=True)
    data.next()  # Skip header


    operating_point = None # Can be loose = 0, medium = 1, tight = 2, discriminator reshaping = 3
    measurement_type = None  # A string representing the type of measurement done to obtain the scale factors
    jet_flavor = None
    for row in data:

        if len(row) != 11:
            continue

        operating_point = int(row[0])
        measurement_type = row[1]
        syst_type = row[2]
        jet_flavor = int(row[3])

        token = get_token(operating_point, measurement_type, jet_flavor)

        if token in all_json_content:
            json_content = all_json_content[token]
        else:
            json_content = {'dimension': 3, 'binning': {'x': [], 'y': [], 'z': []}, 'data': [], 'absolute_errors': True, 'formula': True,
                    'variable': 'y' if operating_point != 3 else 'z'}
            all_json_content[token] = json_content

        json_content_data = json_content['data']

        eta_bin = [float(row[4]), float(row[5])]
        pt_bin = [float(row[6]), float(row[7])]
        discr_bin = [float(row[8]), float(row[9])]
        formula = row[10]

        def get_bin(bin, data):

            for d in data:
                if d['bin'] == bin:
                    return d

            # Not found. Create bin and add it
            d = {'bin': bin, 'values': []}
            data.append(d)

            return d


        x_bin = get_bin(eta_bin, json_content_data)
        y_bin = get_bin(pt_bin, x_bin['values'])

        if syst_type == 'central':
            # Create bin
            z_bin = {'bin': discr_bin, 'value': formula}
            y_bin['values'].append(z_bin)

        else:
            z_bin = get_bin(discr_bin, y_bin['values'])
            z_bin['error_high' if syst_type == 'up' else 'error_low'] = formula

for (operating_point, measurement_type, jet_flavor), json_content in all_json_content.items():
    # Create binning
    def create_binning(root):
        binning = []
        for d in root:
            if len(binning) == 0:
                binning.extend(d['bin'])
            else:
                binning.append(d['bin'][1])

        return binning

    json_content_data = json_content['data']

    json_content['binning']['x'] = create_binning(json_content_data)
    json_content['binning']['y'] = create_binning(json_content_data[0]['values'])
    json_content['binning']['z'] = create_binning(json_content_data[0]['values'][0]['values'])

    import json
    filename = 'BTagging_%s_%s_%s_%s.json' % (operating_point_to_string(operating_point), jet_flavor_to_string(jet_flavor), measurement_type, args.suffix)
    with open(filename, 'w') as j:
        json.dump(json_content, j, indent=2)
