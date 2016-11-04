#ifndef NAVIGATOR_H_
#define NAVIGATOR_H_

typedef struct HyperLink HyperLink;
struct HyperLink {
    char *text;
    char *url;
};

char * get_bodyhtml_from_url(char *url);
HyperLink * select_hyperlinks_from_html(char *html);

#endif
