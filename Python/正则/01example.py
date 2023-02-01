import re

def link_inline(text):
    # expand a proper link around recipe id
    ref = re.compile("\[(.*?)\]\((\d+)\)")
    replacer = r'<a href="https://www.foo.com/recipes?rid=\2">\1</a>'
    return ref.sub(replacer, text)


text = """
2018 was a big year for my sourdough starter and me. Mostly we worked on
 developing this [tangy bread](19928) and
 these [chewy rolls](9843). But we were also just
 content keeping each other company and inspired to bake.
"""

print(link_inline(text))

