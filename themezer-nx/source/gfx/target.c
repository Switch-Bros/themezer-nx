#include "gfx.h"

ShapeLinker_t *CreateSideTargetMenu(RequestInfo_t *rI){
    ShapeLinker_t *out = CreateSideBaseMenu("Typ");

    ShapeLinker_t *list = NULL;
    for (int i = 0; i < 9; i++) {
        if (rI->target == i) {
            SetActiveColorTexture(targetIcons[i]);
        } else {
            SetInactiveColorTexture(targetIcons[i]);
        }
        ShapeLinkAdd(&list, ListItemCreate((rI->target == i) ? COLOR_FILTERACTIVE : COLOR_WHITE, COLOR_WHITE, targetIcons[i], targetOptions[i], NULL), ListItemType);
    }

    ShapeLinkAdd(&out, ListViewCreate(POS(0, 50, 400, SCREEN_H - 100), 60, COLOR_MAINBG, COLOR_CURSOR, COLOR_CURSORPRESS, COLOR_SCROLLBAR, COLOR_SCROLLBARTHUMB, LIST_CENTERLEFT, list, exitFunc, NULL, FONT_TEXT[FSize30]), ListViewType);

    ShapeLinkAdd(&out, ButtonCreate(POS(0, SCREEN_H - 50, 400, 50), COLOR_MAINBG, COLOR_RED, COLOR_WHITE, COLOR_CURSOR, 0, ButtonStyleBottomStrip, "Themezer-NX beenden", FONT_TEXT[FSize25], exitFunc), ButtonType);
    ShapeLinkAdd(&out, GlyphCreate(376, SCREEN_H - 48, BUTTON_PLUS, COLOR_WHITE, FONT_BTN[FSize20]), GlyphType);

    return out;
}

int ShowSideTargetMenu(Context_t *ctx){
    RequestInfo_t *rI = ShapeLinkFind(ctx->all, DataType)->item;
    ShapeLinker_t *menu = CreateSideTargetMenu(rI);
    Context_t menuCtx = MakeMenu(menu, ButtonHandlerBExit, NULL);

    if (menuCtx.selected->type == ListViewType && menuCtx.origin == OriginFunction){
        ListView_t *lv = menuCtx.selected->item;
        int selection = lv->highlight;
        if (rI->target != selection){
            int tempTarget = rI->target;
            int tempPage = rI->page;
            SetDefaultsRequestInfo(rI);
            rI->target = selection;
            if (selection == 8){
                rI->maxDls = 3;
                rI->limit = 12;
            }
            printf("Anfrage wird erstellt...\n");
            if (MakeRequestAsCtx(ctx, rI)){
                rI->target = tempTarget;
                rI->page = tempPage;
            }
        }
    }

    ShapeLinkDispose(&menu);
    return (menuCtx.curOffset == 8 && menuCtx.origin == OriginFunction) ? -1 : 0; 
}