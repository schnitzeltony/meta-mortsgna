def armv7_tune_handler(d):
    tune = d.getVar('DEFAULTTUNE')
    # set MORTSGNA_ARMV7_TUNE="1" in your local.conf to reduce all cortex variants
    # to common tune value
    if d.getVar('MORTSGNA_ARMV7_TUNE') == '1':
        features = d.getVar('TUNE_FEATURES').split()
        if 'armv7a' in features or 'armv7ve'  or 'armv7ve'  or 'cortexa7'  or 'cortexa8'  or 'cortexa9' in features:
            tune = 'armv7athf'
            if 'bigendian' in features:
                tune += 'b'
            if 'neon' in features:
                tune += '-neon'
        else:
            tune = d.getVar('DEFAULTTUNE')
    return tune

