flags = [
'-x',
'c++',
'-Wall',
'-Werror',
'-I./',
]

def FlagsForFile(filename):
    return {
        'flags': flags,
        'do_cache': True
    }
