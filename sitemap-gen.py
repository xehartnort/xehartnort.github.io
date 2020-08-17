import os
import datetime
from jinja2 import Template
import gzip
import argparse
import sys
from urllib.parse import quote


def write_to_sitemap(all_urls, sitemap_template, index):
    # We have parsed url_per_file lines
    template = Template(sitemap_template)
    # Render each row / column in the sitemap
    sitemap_output = template.render(pages=list_of_urls)
    # Create a filename for each sitemap like: sitemap_0.xml.gz, sitemap_1.xml.gz, etc.
    filename = 'sitemap_' + str(index) + '.xml.gz'
    # Write the file to your current folder, not appending???
    with gzip.open(filename, 'wt') as f:
        f.write(sitemap_output)


# Create a Sitemap Template to Populate
sitemap_template = '''<?xml version="1.0" encoding="UTF-8"?>
<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">
    {% for page in pages %}
    <url>
        <loc>{{page[0]|safe}}</loc>
        <lastmod>{{page[1]}}</lastmod>
    </url>
    {% endfor %}
</urlset>'''

today = datetime.datetime.now().strftime('%Y-%m-%d')

if __name__ == "__main__":
    description = "This script collects all the urls listed in a file and generates as many sitemap.xml files as required"
    parser = argparse.ArgumentParser(description)
    parser.add_argument("-n", "--url-per-file", type=int, dest="url_per_file",
                        help="Number of URL per sitemap file (default 50.000)", default=50000)
    parser.add_argument("-u", "--url-list", type=str, dest="url_list",
                        help="Path to file which contains all the URLs", required=True)
    parser.add_argument("-wr", "--write-robots-txt",
                        type=bool, dest="write_robots")

    args = parser.parse_args(sys.argv[1:])

    url_per_file = args.url_per_file
    url_list = args.url_list
    write_robots = args.write_robots or False

    list_of_urls = []

    with open(url_list, 'r') as fin:
        file_number = 0
        for i, lin in enumerate(fin):
            array_i = i % url_per_file
            if array_i != 0 or i == 0:
                encoded_url = quote(lin.strip(), safe=':/')
                list_of_urls.append((encoded_url, today))
            else:
                write_to_sitemap(list_of_urls, sitemap_template, file_number)
                file_number += 1
                list_of_urls = []
        if len(list_of_urls) > 0:
            write_to_sitemap(list_of_urls, sitemap_template, file_number)

    if write_robots:
        with open("robots.txt", 'w') as frobots:
            for i in range(file_number+1):
                frobots.write("Sitemap: sitemap_{}.xml.gz\n".format(i))
            frobots.write("\nUser-agent: *\nDisallow:")
