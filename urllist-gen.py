import argparse
import sys
import os

if __name__ == "__main__":
    description = "This script collects all the urls that you want to add in your sitemap.xml"
    parser = argparse.ArgumentParser(description)
    parser.add_argument("-u", "--url", type=str, dest="url",
                        help="Root url, files path are appended to this url", required=True)
    parser.add_argument("-p", "--path", type=str, dest="path",
                        help="Root path to folder which contains the files", default='.')
    parser.add_argument('-sf', '--skip-files', dest="skip_ext", nargs='+',
                        help='List of files or extensions to avoid in the listing', default=[])
    # parser.add_argument('-sd', '--skip-dirs', dest="skip_dirs", nargs='+',
    #                    help='List of dirs to avoid in the listing', default=[])
    args = parser.parse_args(sys.argv[1:])

    path = args.path
    url = args.url
    skip_ext = args.skip_ext
    #skip_dirs = args.skip_dirs

    if url[-1] == "/":
        url = url[:-1]
    with open('url_list.txt', 'w') as out:
        out.write(url+"\n")
        for (dirpath, dirnames, files) in os.walk(path):
            # Remove relative path marks
            while dirpath[0] == '.' or dirpath[0] == os.path.sep:
                dirpath = dirpath[1:]
                if len(dirpath) == 0:
                    break

            for f in files:
                skip = False
                for ext in skip_ext:
                    if f.endswith(ext):
                        skip = True
                        break
                if skip:
                    continue
                out.write(url + os.path.sep + dirpath + os.path.sep + f + "\n")
