file="${1}"
filename="${file%.*}"
cat "${file}" | sed 's/<{/\&lang;/g' | sed 's/}>/\&rang;/g' | perl Markdown.pl | sed 's/\&amp;lang;/\&lang;/g' | sed 's/\&amp;rang;/\&rang;/g' > "${filename}.html"