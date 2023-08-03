

```c
#define UI_STYLE_DEFINE(name, state, code) \
static void name(lv_style_t *this) { code }

// 或者直接 ...
// #define UI_STYLE_DEFINE(name, state, ...) \
// static void name(lv_style_t *this) { __VA_ARGS__ }

UI_STYLE_DEFINE(button, LV_STATE_PRESSED,
    lv_style_set_bg_color(this, lv_color_hex(0x181C35));
    lv_style_set_bg_opa(this, LV_OPA_100);
    lv_style_set_border_opa(this, LV_OPA_0);
)
```

