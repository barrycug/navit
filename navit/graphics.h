/**
 * Navit, a modular navigation system.
 * Copyright (C) 2005-2008 Navit Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

/** @file
 *
 * @brief Exported functions / structures for the graphics subsystem.
 */

#ifndef NAVIT_GRAPHICS_H
#define NAVIT_GRAPHICS_H

#ifdef __cplusplus
extern "C" {
#endif
struct attr;
struct point;
struct container;
struct color;
struct graphics;
struct graphics_gc;
struct graphics_font;
struct graphics_image;
struct transformation;
struct display_list;
struct mapset;

/* This enum must be synchronized with the constants in NavitGraphics.java. */
enum draw_mode_num {
	draw_mode_begin, draw_mode_end
};

struct graphics_priv;
struct graphics_font_priv;
struct graphics_image_priv;
struct graphics_gc_priv;
struct graphics_font_methods;
struct graphics_gc_methods;
struct graphics_image_methods;

enum graphics_image_type {
	graphics_image_type_unknown=0,
};

struct graphics_image_buffer {
	char magic[8]; /* buffer:\0 */
	enum graphics_image_type type;
	void *start;
	int len;
};

/** Magic value for unset/unspecified width/height. */
#define IMAGE_W_H_UNSET (-1)

/** @brief The functions to be implemented by graphics plugins.
 *
 * This struct lists the functions that Navit graphics plugins must implement.
 * The plugin must supply its list of function implementations from its plugin_init() function.
 * @see graphics_gtk_drawing_area#plugin_init()
 * @see graphics_android#plugin_init()
 */
struct graphics_methods {
	void (*graphics_destroy)(struct graphics_priv *gr);
	void (*draw_mode)(struct graphics_priv *gr, enum draw_mode_num mode);
	void (*draw_lines)(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point *p, int count);
	void (*draw_polygon)(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point *p, int count);
	void (*draw_rectangle)(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point *p, int w, int h);
	void (*draw_circle)(struct graphics_priv *gr, struct graphics_gc_priv *gc, struct point *p, int r);
	void (*draw_text)(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct graphics_gc_priv *bg, struct graphics_font_priv *font, char *text, struct point *p, int dx, int dy);
	void (*draw_image)(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct point *p, struct graphics_image_priv *img);
	void (*draw_image_warp)(struct graphics_priv *gr, struct graphics_gc_priv *fg, struct point *p, int count, struct graphics_image_priv *img);
	void (*draw_drag)(struct graphics_priv *gr, struct point *p);
	struct graphics_font_priv *(*font_new)(struct graphics_priv *gr, struct graphics_font_methods *meth, char *font,  int size, int flags);
	struct graphics_gc_priv *(*gc_new)(struct graphics_priv *gr, struct graphics_gc_methods *meth);
	void (*background_gc)(struct graphics_priv *gr, struct graphics_gc_priv *gc);
	struct graphics_priv *(*overlay_new)(struct graphics_priv *gr, struct graphics_methods *meth, struct point *p, int w, int h, int wraparound);
	/** @brief Load an image from a file.
	 *
	 * @param gr graphics object
	 * @param meth output parameter for graphics methods object
	 * @param path file name/path of image to load
	 * @param w In: width to scale image to, or IMAGE_W_H_UNSET for original width.
	 * Out: Actual width of returned image.
	 * @param h heigth; see w
	 * @param hot output parameter for image hotspot
	 * @param rotate angle to rotate the image, in 90 degree steps (not supported by all plugins).
	 * @return pointer to allocated image, to be freed by image_free()
	 * @see image_free()
	 */
	struct graphics_image_priv *(*image_new)(struct graphics_priv *gr, struct graphics_image_methods *meth, char *path, int *w, int *h, struct point *hot, int rotation);
	void *(*get_data)(struct graphics_priv *gr, const char *type);
	void (*image_free)(struct graphics_priv *gr, struct graphics_image_priv *priv);
	void (*get_text_bbox)(struct graphics_priv *gr, struct graphics_font_priv *font, char *text, int dx, int dy, struct point *ret, int estimate);
	void (*overlay_disable)(struct graphics_priv *gr, int disable);
	void (*overlay_resize)(struct graphics_priv *gr, struct point *p, int w, int h, int wraparound);
	int (*set_attr)(struct graphics_priv *gr, struct attr *attr);
};


struct graphics_font_methods {
	void (*font_destroy)(struct graphics_font_priv *font);
};

struct graphics_font {
	struct graphics_font_priv *priv;
	struct graphics_font_methods meth;
};

struct graphics_gc_methods {
	void (*gc_destroy)(struct graphics_gc_priv *gc);
	void (*gc_set_linewidth)(struct graphics_gc_priv *gc, int width);
	void (*gc_set_dashes)(struct graphics_gc_priv *gc, int width, int offset, unsigned char dash_list[], int n);
	void (*gc_set_foreground)(struct graphics_gc_priv *gc, struct color *c);
	void (*gc_set_background)(struct graphics_gc_priv *gc, struct color *c);
};

/**
 * @brief graphics context
 * A graphics context encapsulates a set of drawing parameters, such as
 * linewidth and drawing color.
 */
struct graphics_gc {
	struct graphics_gc_priv *priv;
	struct graphics_gc_methods meth;
	struct graphics *gra;
};

struct graphics_image_methods {
	void (*image_destroy)(struct graphics_image_priv *img);
};

struct graphics_image {
	struct graphics_image_priv *priv;
	struct graphics_image_methods meth;
	int width;
	int height;
	struct point hot;
};

struct graphics_data_image {
	void *data;
	int size;
};

/* prototypes */
enum attr_type;
enum draw_mode_num;
enum item_type;
struct attr;
struct attr_iter;
struct callback;
struct color;
struct displayitem;
struct displaylist;
struct displaylist_handle;
struct graphics;
struct graphics_font;
struct graphics_gc;
struct graphics_image;
struct item;
struct itemgra;
struct layout;
struct mapset;
struct point;
struct point_rect;
struct transformation;
int graphics_set_attr(struct graphics *gra, struct attr *attr);
void graphics_set_rect(struct graphics *gra, struct point_rect *pr);
struct graphics *graphics_new(struct attr *parent, struct attr **attrs);
int graphics_get_attr(struct graphics *this_, enum attr_type type, struct attr *attr, struct attr_iter *iter);
struct graphics *graphics_overlay_new(struct graphics *parent, struct point *p, int w, int h, int wraparound);
void graphics_overlay_resize(struct graphics *this_, struct point *p, int w, int h, int wraparound);
void graphics_init(struct graphics *this_);
void *graphics_get_data(struct graphics *this_, const char *type);
void graphics_add_callback(struct graphics *this_, struct callback *cb);
void graphics_remove_callback(struct graphics *this_, struct callback *cb);
struct graphics_font *graphics_font_new(struct graphics *gra, int size, int flags);
struct graphics_font *graphics_named_font_new(struct graphics *gra, char *font, int size, int flags);
void graphics_font_destroy(struct graphics_font *gra_font);
void graphics_free(struct graphics *gra);
void graphics_font_destroy_all(struct graphics *gra);
struct graphics_gc *graphics_gc_new(struct graphics *gra);
void graphics_gc_destroy(struct graphics_gc *gc);
void graphics_gc_set_foreground(struct graphics_gc *gc, struct color *c);
void graphics_gc_set_background(struct graphics_gc *gc, struct color *c);
void graphics_gc_set_linewidth(struct graphics_gc *gc, int width);
void graphics_gc_set_dashes(struct graphics_gc *gc, int width, int offset, unsigned char dash_list[], int n);
struct graphics_image *graphics_image_new_scaled(struct graphics *gra, char *path, int w, int h);
struct graphics_image *graphics_image_new_scaled_rotated(struct graphics *gra, char *path, int w, int h, int rotate);
struct graphics_image *graphics_image_new(struct graphics *gra, char *path);
void graphics_image_free(struct graphics *gra, struct graphics_image *img);
void graphics_draw_mode(struct graphics *this_, enum draw_mode_num mode);
void graphics_draw_lines(struct graphics *this_, struct graphics_gc *gc, struct point *p, int count);
void graphics_draw_circle(struct graphics *this_, struct graphics_gc *gc, struct point *p, int r);
void graphics_draw_rectangle(struct graphics *this_, struct graphics_gc *gc, struct point *p, int w, int h);
void graphics_draw_rectangle_rounded(struct graphics *this_, struct graphics_gc *gc, struct point *plu, int w, int h, int r, int fill);
void graphics_draw_text(struct graphics *this_, struct graphics_gc *gc1, struct graphics_gc *gc2, struct graphics_font *font, char *text, struct point *p, int dx, int dy);
void graphics_get_text_bbox(struct graphics *this_, struct graphics_font *font, char *text, int dx, int dy, struct point *ret, int estimate);
void graphics_overlay_disable(struct graphics *this_, int disable);
int  graphics_is_disabled(struct graphics *this_);
void graphics_draw_image(struct graphics *this_, struct graphics_gc *gc, struct point *p, struct graphics_image *img);
int graphics_draw_drag(struct graphics *this_, struct point *p);
void graphics_background_gc(struct graphics *this_, struct graphics_gc *gc);
void graphics_draw_text_std(struct graphics *this_, int text_size, char *text, struct point *p);
char *graphics_icon_path(const char *icon);
void graphics_draw_itemgra(struct graphics *gra, struct itemgra *itm, struct transformation *t, char *label);
void graphics_displaylist_draw(struct graphics *gra, struct displaylist *displaylist, struct transformation *trans, struct layout *l, int flags);
void graphics_draw(struct graphics *gra, struct displaylist *displaylist, struct mapset *mapset, struct transformation *trans, struct layout *l, int async, struct callback *cb, int flags);
int graphics_draw_cancel(struct graphics *gra, struct displaylist *displaylist);
struct displaylist_handle *graphics_displaylist_open(struct displaylist *displaylist);
struct displayitem *graphics_displaylist_next(struct displaylist_handle *dlh);
void graphics_displaylist_close(struct displaylist_handle *dlh);
struct displaylist *graphics_displaylist_new(void);
void graphics_displaylist_destroy(struct displaylist *displaylist);
struct map_selection *displaylist_get_selection(struct displaylist *displaylist);
GList *displaylist_get_clicked_list(struct displaylist *displaylist, struct point *p, int radius);
struct item *graphics_displayitem_get_item(struct displayitem *di);
int graphics_displayitem_get_coord_count(struct displayitem *di);
char *graphics_displayitem_get_label(struct displayitem *di);
int graphics_displayitem_get_displayed(struct displayitem *di);
int graphics_displayitem_get_z_order(struct displayitem *di);
int graphics_displayitem_within_dist(struct displaylist *displaylist, struct displayitem *di, struct point *p, int dist);
void graphics_add_selection(struct graphics *gra, struct item *item, enum item_type type, struct displaylist *dl);
void graphics_remove_selection(struct graphics *gra, struct item *item, enum item_type type, struct displaylist *dl);
void graphics_clear_selection(struct graphics *gra, struct displaylist *dl);
/* end of prototypes */
#ifdef __cplusplus
}
#endif

#endif

