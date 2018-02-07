import re
import string

dictionary = {
        'AA ': 'A',
        'AE ': 'A',
        'AH ': 'A',
        'AO ': 'O',
        'AW ': 'AU',
        'AY ': 'AI',
        'EH ': 'E',
        'ER ': 'A-',
        'EY ': 'E-',
        'IH ': 'I',
        'IY ': 'I-',
        'OW ': 'O-',
        'OY ': 'OI',
        'UH ': 'U-',
        'UW ': 'U-',
        'B ' : 'B',
        'CH ': 'T',
        'D ' : 'D',
        'DH ': 'Z',
        'F ' : 'F',
        'G ' : 'G',
        'HH ': 'H',
        'JH ': 'J',  #
        'K ' : 'K',
        'L ' : 'R',
        'M ' : 'M',
        'N ' : 'N',
        'NG ': 'N',
        'P ' : 'P',
        'R ' : 'R',
        'S ' : 'S',
        'SH ': 'S',
        'T ' : 'T',
        'TH ': 'S',
        'V ' : 'V',  #
        'W ' : 'U',
        'Y ' : 'I',
        'Z ' : 'Z',
        'ZH ': 'J',  #
        }

def e2j(w):
    w = re.sub('|'.join(dictionary.keys()),lambda x: dictionary[x.group(0)],w)
    w = re.sub('([KGSZHFPBMR])([^AIUEO])',lambda x: x.group(1)+'U'+x.group(2),w)
    w = re.sub('([KGSZHFPBMR])([^AIUEO])',lambda x: x.group(1)+'U'+x.group(2),w)
    w = re.sub('([^AIUEO])N([^AIUEO])',lambda x: x.group(1)+'NU'+x.group(2),w)
    w = re.sub('([TD])([^AIUEO])',lambda x: x.group(1)+'O'+x.group(2),w)
    w = re.sub('J([^I])',lambda x: 'JI'+group(1),w)
    return w
