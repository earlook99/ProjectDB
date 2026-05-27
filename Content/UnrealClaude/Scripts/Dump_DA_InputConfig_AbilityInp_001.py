import unreal
cfg=unreal.load_asset("/Game/Input/DA_InputConfig")
rows=cfg.get_editor_property("AbilityInputActions")
print("ROWS:",len(rows))
for r in rows:
    ia=r.get_editor_property("InputAction")
    tag=r.get_editor_property("InputTag")
    print((ia.get_name() if ia else "None"),"->",str(tag))
