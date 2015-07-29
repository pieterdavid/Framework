#! /bin/env python

import argparse
import pickle

def format_eta_bin(eta_bin):
    return 'ptabseta<%.1f' % (eta_bin[1]) if (eta_bin[0] == 0) else 'ptabseta%.1f-%.1f' % (eta_bin[0], eta_bin[1])

parser = argparse.ArgumentParser()
parser.add_argument('pkl', help='Pickle file containing muon scale factors')
parser.add_argument('-s', '--suffix', help='Suffix to append at the end of the output filename', required=True)

args = parser.parse_args()

eta_binning = [0, 0.9, 1.2, 2.1, 2.4]

with open(args.pkl) as f:
    d = pickle.load(f)

    for wp, wp_data in d.items():

        json_content = {'dimension': 2, 'binning': {'x': eta_binning, 'y': []}, 'data': []}
        json_content_data = json_content['data']

        for i in range(0, len(eta_binning) - 1):

            eta_bin = format_eta_bin([eta_binning[i], eta_binning[i + 1]])

            if eta_bin not in wp_data:
                continue

            eta_data = {'bin': [eta_binning[i], eta_binning[i + 1]], 'values': []}

            pt_binning = []
            for pt_bin_str, d in wp_data[eta_bin].items():

                pt_bin = pt_bin_str.split('_')
                pt_bin = [float(x) for x in pt_bin]
                pt_binning.extend(pt_bin)
                content = d['data/mc']

                pt_data = {'bin': pt_bin, 'value': content['efficiency_ratio'], 'error_low': content['err_low'], 'error_high': content['err_hi']}

                eta_data['values'].append(pt_data)

            json_content_data.append(eta_data)
            # Remove duplicates
            pt_binning = list(set(pt_binning))
            pt_binning.sort()

            json_content['binning']['y'] = pt_binning

        # Save JSON file
        filename = 'Muon_%s_%s.json' % (wp, args.suffix)
        with open(filename, 'w') as j:
            import json
            json.dump(json_content, j, indent=2)
