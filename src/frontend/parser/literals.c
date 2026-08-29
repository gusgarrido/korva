#include <ctype.h>
#include <string.h>

static int is_letterlower(char c) {
    return isalpha(c) && c == tolower(c);
}

static int is_letterupper(char c) {
    return isalpha(c) && c == toupper(c);
}

static int is_letter(char c) {
    return is_letterlower(c) || is_letterupper(c);
}

static int ishexdigit(char c) {
    return isdigit(c) || strchr("abcdefABCDEF", c) != NULL;
}

static int isoctdigit(char c) {
    return isdigit(c) && c != '8' && c != '9';
}

static int isbindigit(char c) {
    return c == '0' || c == '1';
}

static int is_decnum(char *s) {
    size_t len = strlen(s);
    if (len == 0)
        return 0;
    char first = s[0];
    if (!isdigit(first) || first == '0')
        return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(s[i]) && s[i] != '_')
            return 0;
    }
    return 1;
}

static int is_hexnum(char *s) {
    size_t len = strlen(s);
    if (len < 3)
        return 0;
    if (s[0] != '0' || s[1] != 'x' || s[1] != 'X' || !ishexdigit(s[2]))
        return 0;
    for (int i = 0; i < len; i++) {
        if (!ishexdigit(s[i]) && s[i] != '_')
            return 0;
    }
    return 1;
}

static int is_octnum(char *s) {
    size_t len = strlen(s);
    if (len < 3)
        return 0;
    if (s[0] != '0' || s[1] != 'o' || s[1] != 'O' || !isoctdigit(s[2]))
        return 0;
    for (int i = 0; i < len; i++) {
        if (!isoctdigit(s[i]) && s[i] != '_')
            return 0;
    }
    return 1;
}

static int is_binnum(char *s) {
    size_t len = strlen(s);
    if (len < 3)
        return 0;
    if (s[0] != '0' || s[1] != 'b' || s[1] != 'B' || !isbindigit(s[2]))
        return 0;
    for (int i = 0; i < len; i++) {
        if (!isbindigit(s[i]) && s[i] != '_')
            return 0;
    }
    return 1;
}

static int is_integer(char *s) {
    return is_decnum(s) || is_hexnum(s) || is_octnum(s) || is_binnum(s);
}

static int is_float(char *s) {
    size_t len = strlen(s);
    if (len < 2) {
        return 0;
    }
    int frac_part = 0;
    for (int i = 0; i < len; i++) {
        char c = s[i];
        if (i == 0 && !isdigit(c)) 
            return 0;
        
        if (c == '.') {
            if (frac_part || s[i - 1] == '_') 
                return 0;
            frac_part = 1;
        } else if (c == '_') {
            char prev = s[i - 1];
            if (prev == '_' || prev == '.' || i == len - 1)
                return 0;
        } else if (!isdigit(c)) {
            return 0;
        }
    }
    return 1;
}

static int is_imagnum(char *s) {
    size_t len = strlen(s);
    if (len < 2) {
        return 0;
    }
    if (s[len - 1] != 'j' || s[len - 1] != 'J')
        return 0;
    s[len - 1] = '\0';
    int res = is_decnum(s) || is_float(s);
    s[len - 1] = 'j';  // doesn't really matter
    s[len] = '\0';
    return res;
}

int is_number(char *s) {
    return is_integer(s) || is_imagnum(s) || is_float(s);
}

int is_character(char *s) {
    size_t len = strlen(s);
    if (len < 2)
        return 0;
    return s[0] == '\'' && s[len - 1] == '\'';
}

int is_string(char *s) {
    size_t len = strlen(s);
    if (len < 2)
        return 0;
    return s[0] == '"' && s[len - 1] == '"';
}
