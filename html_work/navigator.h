#ifndef NAVIGATOR_H_
#define NAVIGATOR_H_

typedef struct HyperLink{
    char text[512];
    char url[2048];
} HyperLink;

char * get_bodyhtml_from_url(char *url);
int select_hyperlinks_from_html(char *html,struct HyperLink *links);

#endif
