#ifndef NAVIGATOR_H_
#define NAVIGATOR_H_

typedef struct HyperLink{
    char *text;
    char *url;
} HyperLink;

char * get_bodyhtml_from_url(char *url);
int select_hyperlinks_from_html(char *html,struct HyperLink *links);

#endif
