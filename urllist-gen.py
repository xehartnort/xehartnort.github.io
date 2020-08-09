import argparse
import sys
import os

if __name__ == "__main__":
    description = "This script collects all the urls that you want to add in your sitemap.xml"
    parser = argparse.ArgumentParser(description)
    parser.add_argument("-p", "--path", type=str, dest="path", help="Root path to folder which contains the files", default='./exámenes')
    parser.add_argument("-u", "--url", type=str, dest="url", help="Root url, files path are appended to this url", required=True)
    args = parser.parse_args(sys.argv[1:])

    path = args.path
    url = args.url

    if url[-1] == "/":
        url = url[:-1]
    with open('url_list.txt', 'w') as out:
        for (dirpath, dirnames, files) in os.walk(path):
            # Remove relative path marks
            while dirpath[0] == '.' or dirpath[0] == os.path.sep:
                dirpath = dirpath[1:]

            for f in files:
                out.write(url + os.path.sep + dirpath + os.path.sep + f + "\n")
