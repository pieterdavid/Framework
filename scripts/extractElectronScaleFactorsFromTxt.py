#! /bin/env python

import math
import argparse
import ROOT

def format_eta_bin(eta_bin):
    return 'ptabseta<%.1f' % (eta_bin[1]) if (eta_bin[0] == 0) else 'ptabseta%.1f-%.1f' % (eta_bin[0], eta_bin[1])

parser = argparse.ArgumentParser()
parser.add_argument('file', help='Txt file containing electron scale factors')
parser.add_argument('-s', '--suffix', help='Suffix to append at the end of the output filename', required=True)

args = parser.parse_args()


# Parse input files
# Structure is
# eta_low eta_up pt_low pt_up eff_data eff_data_err eff_mc eff_mc_err bkg_mod_err sig_mod_err range_err mc_err pu_err tag_sel_err

efficiencies = {}

with open(args.file, 'r') as f:
    eta_binning = []
    pt_binning = []

    for line in f:
        data = line.strip().split()

        eta_bin = (float(data[0]), float(data[1]))
        pt_bin = (float(data[2]), float(data[3]))

        if len(eta_binning) == 0:
            eta_binning.extend(eta_bin)
        else:
            eta_binning.append(eta_bin[1])

        if len(pt_binning) == 0:
            pt_binning.extend(pt_bin)
        else:
            pt_binning.append(pt_bin[1])

        eff = {
                'data': float(data[4]),
                'data_err': float(data[5]),
                'mc': float(data[6]),
                'mc_err': float(data[7]),
                }

        if not eta_bin in efficiencies:
            efficiencies[eta_bin] = {}

        efficiencies[eta_bin][pt_bin] = eff

eta_binning = sorted(list(set(eta_binning)))
pt_binning = sorted(list(set(pt_binning)))

json_content = {'dimension': 2, 'binning': {'x': eta_binning, 'y': pt_binning}, 'data': [], 'error_type': 'absolute'}
json_content_data = json_content['data']


for i in range(0, len(eta_binning) - 1):
    eta_data = {'bin': [eta_binning[i], eta_binning[i + 1]], 'values': []}
    eta_bin = (eta_binning[i], eta_binning[i + 1])
    for j in range(0, len(pt_binning) - 1):
        pt_bin = (pt_binning[j], pt_binning[j + 1])
        eff = efficiencies[eta_bin][pt_bin]

        scale_factor = eff['data'] / eff['mc']

        scale_factor_error = math.sqrt(eff['data_err']**2 / eff['data']**2 + eff['mc_err']**2 / eff['mc']**2)

        pt_data = {'bin': [pt_binning[j], pt_binning[j + 1]], 'value': scale_factor, 'error_low': scale_factor_error, 'error_high': scale_factor_error}

        eta_data['values'].append(pt_data)

    json_content_data.append(eta_data)

# Save JSON file
import os
wp = os.path.splitext(os.path.basename(args.file))[0]
filename = 'Electron_%s_%s.json' % (wp, args.suffix)
with open(filename, 'w') as j:
    import json
    json.dump(json_content, j, indent=2)
