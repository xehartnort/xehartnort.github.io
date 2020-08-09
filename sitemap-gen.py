import os
import datetime
from jinja2 import Template
import gzip
import argparse
import sys

# Set-Up Maximum Number of URLs (recommended max 50,000)
# Added 20 here because I am using a small template of URLs and I want to show an example
url_per_file = 50000

# Create a Sitemap Template to Populate
sitemap_template = '''<?xml version="1.0" encoding="UTF-8"?>
<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">
    {% for page in pages %}
    <url>
        <loc>{{page[0]|safe}}</loc>
        <lastmod>{{page[1]}}</lastmod>
        <changefreq>{{page[2]}}</changefreq>
        <priority>{{page[3]}}</priority>
    </url>
    {% endfor %}
</urlset>'''

today = datetime.datetime.now().strftime('%Y-%m-%d')

if __name__ == "__main__":
    description = "This script collects all the urls listed in a file and generates as many sitemap.xml files as required"
    parser = argparse.ArgumentParser(description)
    parser.add_argument("-n", "--url-per-file", type=int, dest="url_per_file", help="Number of URL per sitemap file", default=300)
    parser.add_argument("-u", "--url-list", type=str, dest="url_list", help="Path to file which contains all the URLs", required=True)
    args = parser.parse_args(sys.argv[1:])

    url_per_file = args.url_per_file
    url_list = args.url_list

    list_of_urls = [[0, '', '', '']] * url_per_file

    with open(url_list, 'r') as fin:
        with open("robots.txt", 'w') as frobots:
            file_number = 0
            for i, lin in enumerate(fin):
                array_i = i % url_per_file
                if array_i != 0 or i == 0:
                    # Remove whitespace and return chars
                    list_of_urls[array_i][0] = lin.strip()
                    # Set Today's Date to add as Lastmod
                    list_of_urls[array_i][1] = today
                    # Set change frequency
                    list_of_urls[array_i][2] = 'daily'
                    # Set priority
                    list_of_urls[array_i][3] = '1.0'
                else:
                    # We have parsed url_per_file lines
                    template = Template(sitemap_template)
                    # Render each row / column in the sitemap
                    sitemap_output = template.render(pages=list_of_urls)
                    # Create a filename for each sitemap like: sitemap_0.xml.gz, sitemap_1.xml.gz, etc.
                    filename = 'sitemap_' + str(file_number) + '.xml.gz'
                    # Write the file to your current folder
                    with gzip.open(filename, 'wt') as f:
                        f.write(sitemap_output)

                    frobots.write("Sitemap: {}\n".format(filename))

                    file_number += 1
            # As the number of lines in the input file is unknown, we need to
            # ensure that if url_per_file is bigger than
            # the number of lines, at least a sitemap file is generated
            if file_number == 0:
                # We have parsed url_per_file lines
                template = Template(sitemap_template)
                # Render each row / column in the sitemap
                sitemap_output = template.render(pages=list_of_urls)
                # Create a filename for each sitemap like: sitemap_0.xml.gz, sitemap_1.xml.gz, etc.
                filename = 'sitemap.xml.gz'
                # Write the file to your current folder
                with gzip.open(filename, 'wt') as f:
                    f.write(sitemap_output)

                frobots.write("Sitemap: {}\n".format(filename))

                file_number += 1

            frobots.write("User-agent: *\nDisallow:")
