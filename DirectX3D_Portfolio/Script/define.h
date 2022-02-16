#pragma once

enum class ITEM
{
    // 일반 등급
    // 단검
    SCISSORS,
    FOUNTAIN_PEN,
    KITCHEN_KNIFE,

    // 양손검
    RUSTY_SWORD,

    // 도끼
    PICKAXE,
    HARCHET,

    // 쌍검
    TWIN_BLADES,

    // 방망이
    BRANCH,
    SHORT_ROD,

    //망치
    HAMMER,

    //글러브
    BRASS_KNUCKLES,
    COTTON_GLOVES,

    //통파
    BAMBOO,

    // 창
    SHORT_SPEAR,

    // 권총
    WALTHER_PPK,

    // 라이플
    FEDOROVA,

    // 저격총
    LONG_RIFLE,

    // 쌍절곤
    STEEL_CHAIN,

    // 기타 무기
    IRON_BALL,
    RAZOR,
    PLAYING_CARD,
    CHALK,
    SHORT_CROSSBOW,
    NEEDLE,

    // 상의
    WINDBREAKER,
    MONKS_ROBE,
    WETSUIT,
    FABRIC_ARMOR,

    // 머리
    HAIRBAND,
    HAT,
    BIKE_HELMET,

    // 팔
    WATCH,
    BANDAGE,
    BRACELET,

    // 다리
    SLIPPERS,
    RUNNING_SHOES,
    TIGHTS,

    // 장신구
    FEATHER,
    FLOWER,
    RIBBON,
    FAN,
    BUDDHIST_SCRIPTURE,
    BOX,
    HOLY_GRAIL,
    CROSS,
    BINOCULARS,

    // 음식
    POTATO,
    COD,
    LEMON,
    GARLIC,
    CARP,
    BREAD,
    MEAT,
    EGG,
    RAMEN,
    ORIENTAL_HUB,
    CHOCOLATE,

    // 음료
    HONEY,
    WATER,
    ICE,
    WHISKEY,
    COFFEE,
    CARBONATED_WATER,
    MILK,

    // 재료
    STONE,
    GLASS_BOTTLE,
    NAIL,
    LEATHER,
    TURTLE_SHELL,
    RUBBER,
    SCRAP_METAL,
    LIGHTER,
    LASER_POINTER,
    STALLION_MEDAL,
    BATTERY,
    ALCOHOL,
    OIL,
    CLOTH,
    GEMSTONE,
    GLUE,
    PAPER,
    IRON_ORE,
    CAN,
    GUNPOWDER,
    PIANO_WIRE,

    // 고급 등급
    // 단검
    JAMADHAR,
    MILITARY_KNIFE,
    SCALPEL,

    // 양손검
    SHAMSHIR,
    PLASMA_SWORD,

    // 쌍검
    WROUGHT_SWORDS,

    //글러브
    LEATHER_GLOVES,
    IRON_KNUCKLES,

    //통파
    WOODEN_TONFA,

    // 창
    BAMBOO_SPEAR,

    // 권총
    MAGNUM_PYTHON,
    BERETTA_M92F,

    // 라이플
    STG_44,

    // 저격총
    SPRINGFIELD,

    // 쌍절곤
    NUNCHAKU,

    // 상의
    LEATHER_ARMOR,
    LEATHER_JACKET,
    TURTLE_DOBOK,
    MILITARY_SUIT,
    PATCHED_ROBE,
    DRESS,
    BIKINI,
    DIVING_SUIT,
    DEACON_ROBES,

    // 머리
    MASK,
    CIRCLET,
    BERET,
    CHAIN_COIF,
    SAFETY_HELMET,

    // 팔
    LEATHER_SHIELD,
    SQUAD_LEATHER_ARMBAND,
    BRACER,
    BROKEN_WATCH,

    // 다리
    KNEE_PADS,
    CHAIN_LEGGINGS,
    HIGH_HEELS,
    HEELYS,
    REPAIRED_SLIPPERS,
    BOOTS,

    // 장신구
    SAINTS_RELIC,
    FLOWER_OF_FATE,
    GLASS_PIECES,
    DOLL,
    SNIPER_SCOPE,
    BUDDHA_SARIRA,
    QUIVER,
    FEATHER_DUSTER,
    GILDED_QUILL_FAN,
    SHAMANS_BRONZE,
    DECORATIVE_FLINTLOCK,
    MARKSMAN_HANDBOOK,

    // 음식
    HONEY_COD_STEAK,
    CANNED_COD_LIVER,
    GARLIC_BREAD,
    BUTTER,
    CARP_BREAD,
    CHOCO_PIE,
    ORCHID,
    BACON_AND_GARLIC_STICKS,
    BUN,
    HAMBURGER,
    FISH_FILLET_WITH_EGG,
    CITRUS_CAKE,
    HONEY_GARLIC,
    EGG_BUN,
    CHOCO_ICE_CREAM,

    // 음료
    BOILING_WATER,
    LEMONADE,
    WATER_BOTTLE,
    BAIJIU,
    SOJU,
    ICED_COFFEE,
    COCKTAIL,
    COFFEE_LIQUEUR,
    COLA,
    LATTE,
    HONEY_MILK,
    HIGH_BALL,
    CHOCOLATE_MILK,
    HONEY_WATER,
    ICE_WATER,
    ON_THE_ROCKS,
    COWBOY,

    // 재료
    STEEL,
    OILCLOTH,
    HEATED_OIL,
    DEAD_BATTERY,
    WHITE_POWDER,
    ASH,
    ELETRONIC_PARTS,
    BLUEPRINT,
    IRON_SHEET,
    GOLD,
    HEATED_STONE,
    BARBED_WIRE,
    RUBY,
    HARDCOVER,

    // 희귀 등급
    // 단검
    ROSE_KNIFE,
    SWISS_ARMY_KNIFE,
    CARAPACE_KATAR,

    // 양손검
    KATANA,
    MASAMUNE,
    MURAMASA,
    BASTARD_SWORD,
    JEWEL_SWORD,

    // 쌍검
    FLORENTINE,
    POCKET_ACES,

    //글러브
    GAUNTLET,
    WING_KNUCKLES,
    BONE_GAUNTLET,
    SHATTER_SHALL_GAUNTLET,
    GLASS_KNUCKLES,
    PHOENIX_GLOVES,

    //통파
    POLICE_BATON,
    RYUKYU_TONFA,

    // 창
    BIDENT,
    PIKE,
    HALBERD,
    SHARPENED_SPEAR,

    // 권총
    FN57,
    DOUBLE_REVOLVER_SP,
    MAGNUM_ANACONDA,

    // 라이플
    AK_47,
    M16A1,

    // 저격총
    HARPOON_GUN,
    GOLDEN_RIFLE,
    RAILGUN,

    // 쌍절곤
    SHARPER,
    BLEEDER,

    // 상의
    RIDER_JACKET,
    CHAIN_ARMOR,
    SUIT,
    QIPAO,
    SHEET_METAL_ARMOR,
    HANBOK,
    CARDINAL_ROBES,
    BULLETPROOF_VEST,
    COVERT_AGENT_UNIFORM,

    // 머리
    BALLISTIC_HELMET,
    FIRE_HELMET,
    TIARA,
    CROWN,
    CLOSE_HELM,
    MOTORCYCLE_HELMET,

    // 팔
    SHEATH,
    GOLDEN_BRACELET,
    BAZUBAND,
    BARBED_BLOSSOM,
    STEEL_SHIELD,

    // 다리
    GLACIER_CRAMPONS,
    STEEL_KNEE_PADS,
    COMBAT_BOOTS,
    KILLER_HEELS,

    // 장신구
    WHITE_CRANE_FAN,
    UCHIWA,
    MAGAZINE,
    LACED_QUIVER,
    REVENGE_OF_GOUJIAN,
    BUCCANEER_DOUBLOON,
    HAWKEYE,
    JOLLY_ROGER,
    MUSIC_BOX,
    ACTIVE_CAMOUFLAGE,
    AETHER_FRINGE,
    SCHRODINGERS_BOX,
    VERITAS_LUX_MEA,

    // 음식
    SPICY_FISH_STEW,
    FRENCH_FRIES,
    BAKED_POTATO,
    BAKED_CARP,
    HOLY_WATER,
    GRILLED_CHILEAN_SEA_BASS,
    HOT_RAMEN,
    MOCHA_BREAD,
    SCRAMBLED_EGG,
    CHOCOLATE_CHIP_COOKIES,
    CHOCO_PIE_BOX,
    ORIENTAL_CONCOCTION,
    HONEY_BUTTER,
    FRIED_CHICKEN,
    HEALING_POTION,
    HARD_BOILED_EGG,
    SOGGY_BREAD,
    STEAK,
    BUTTER_FRIED_POTATO,
    FISH_CUTTLET,
    STIR_FRIED_RAMEN,
    COLD_NOODLE,
    BIRTHDAY_CAKE,
    GARLIC_RAMEN,

    // 음료
    KAOLIANG_LIQUOR,
    HOT_HONEY_WATER,
    FLOWER_LIQUOR,
    AMERICANO,
    HERBAL_LIQUOR,
    WHISKEY_COCKTAIL,
    PURIFIED_WATER,
    CAN_OF_COLA,
    HOT_CHOCOLATE,
    WHITE_RUSSIAN,
    TEAR_OF_SELENE,

    // 재료
    TREE_OF_LIFE,
    METEORITE,
    MOONSTONE,
    POISON,
    MOTOR,
    MITHRIL,
    GLASS_PANEL,
    ION_BATTERY,
    CELL_PHONE,

    // 영웅 등급
    // 단검
    CARNWENNAN,
    MOUNTAIN_SLICER,
    VIBROBLADE,
    MAHARAJA,

    // 양손검
    THUAN_THIEN,
    ARONDIGHT,
    EXCALIBUR,
    MONOHOSHIZAO,
    HOVUD,

    // 쌍검
    STARSTEEL_TWIN_SWORDS,
    ASURA,
    DIOSCURI,

    //글러브
    ONE_INCH_PUNCH,
    DIVINE_FIST,
    BLOODWING_KNUCKLES,
    FROST_PETAL_HAND,
    BUDDHAS_PALM,
    BRASIL_GAUNTLET,
    WHITE_CLAW_PUNCH,

    //통파
    TATICAL_TONFA,
    MAI_SOK,
    PLASMA_TONFA,

    // 창
    GENTIAN_SILVER_GUN,
    EIGHTEEN_FOOT_SPEAR,
    COSMIC_BIDENT,
    LANCE_OF_POSEIDON,
    FANGTIAN_HUAJI,
    DRAGON_GUANDAO,

    // 권총
    DEVILS_MARKSMAN,
    ELEGANCE,
    ELECTRON_BLASTER,
    MAGNUM_BOA,

    // 라이플
    GATLING_GUN,
    TYPE_95,
    AK_12,
    XCR,

    // 저격총
    TAC_50,
    NTW_20,
    POLARIS,

    // 쌍절곤
    THE_SMITING_DRAGON,
    VIBRO_NUNCHAKU,
    CERBERUS,

    // 상의
    SUNSET_ARMOR,
    OPTICAL_CAMOUFLAGE_SUIT,
    MITHRIL_ARMOR,
    CRUSADER_ARMOR,
    AMAZONESS_ARMOR,
    DRAGON_DOBOK,
    COMMANDERS_ARMOR,
    BUTLERS_SUIT,
    BATTLE_SUIT,
    EOD_SUIT,
    TUXEDO,
    HIGH_PRIEST_ROBES,
    CHANGPAO,

    // 머리
    MITHRIL_HELM,
    CRYSTAL_TIARA,
    TACTICAL_OPS_HELMET,
    HELM_OF_BANNERET,
    IMPERIAL_CROWN,
    IMPERIAL_BURGONET,
    DIADEM,
    
    // 팔
    SWORD_STOPPER,
    DRAUPNIR,
    MITHRIL_SHIELD,
    VITAL_SIGN_SENSOR,
    CREED_OF_THE_KNIGHT,
    SHEATH_OF_SHAH_JAHAN,
    CUBE_WATCH,
    TINDALOS_BAND,
    NIGHTINGALE,
    PLASMA_ARC,
    SMART_BAND,
    
    // 다리
    FEATHER_BOOTS,
    MAVERICK_RUNNER,
    STRAITJACKET_SNEAKERS,
    MITHRIL_BOOTS,
    BUCEPHALUS,
    WHITE_RHINOS,
    TACHYON_BRACE,
    IRON_MAIDEN,
    
    // 장신구
    MOONLIGHT_PENDANT,
    GLACIAL_ICE,
    TRUE_SAMADHI_FIRE,
    LUNAR_EMBRACE,
    MITHIRIL_QUIVER,
    
    // 음식
    ZEN_VITALITY,
    FISH_AND_CHIPS,
    
    // 재료
    VF_BLOOD_SAMPLE,
    FORCE_CORE,

    // 전설 등급
    // 단검
    FRAGARACH,

    // 양손검
    LAEVATEINN,
    AURORA_LONGSWORD,
    DAINSLEIF,

    // 쌍검
    LLOIGOR_N_ZAHR,
    SPRING_AND_AUTUMN,

    //글러브
    MARK_OF_THE_PHOENIX,
    PERMAFROST,

    //통파
    OBSIDIAN_JITTE,

    // 창
    BLAZING_LANCE,
    SPEAR_OF_LONGINUS,

    // 권총
    KELTE,

    // 라이플
    JUDGEMENT,
    AGNI,

    // 저격총
    THE_DEADLY_RAY,
    BLACKFIRE_CANNON,

    // 쌍절곤
    HYDRA,

    // 상의
    BLAZING_DRESS,
    KABANA,
    QUEEN_OF_HEARTS,
    HOLY_ORDERS,
    BURGUNDY_47,

    // 머리
    LAUREL_WREATH,
    CHINESE_OPERA_MASK,
    ELYSIAN_HALO,
    FALLEN_PEGASUS,
    
    // 팔
    TELLURIAN_TIMEPIECE,
    RADER,
    AUTO_ARMS,
    
    // 다리
    EOD_BOOTS,
    GLACIAL_SHOES,
    BOOTS_OF_HERMES,
    RED_SHOES,
    
    // 장신구
    EMERALD_TABLET,
    SANGUINE_GUNBAI,
    KUNDALA,

    END
};

enum class ITEM_TYPE
{
    DAGGER,
    GLOVES,
    NUNCHAKU,
    TONFA,
    BAT,
    WHIP,
    HAMMER,
    THROW,
    SHURIKEN,
    BOW,
    CROSSBOW,
    PISTOL,
    ASSAULT_RIFLE,
    SNIPER_RIFLE,
    AXE,
    TWO_HANDED_SWORD,
    DUAL_SWORD,
    SPEAR,
    RAPIER,
    CHEST,
    HEAD,
    ARM,
    LEG,
    ACCESSORY,
    FOOD,
    BEVERAGE,
    MATERIAL,
    END
};

enum class ITEM_GRADE
{
    NONE,
    GRAY,
    GREEN,
    BLUE,
    PURPLE,
    YELLOW
};

enum class APPEAR
{
    // 등장 위치
    ALLEY,
    ARCHERY_RANGE,
    AVENUE,
    BEACH,
    CEMETERY,
    CHAPEL,
    DOCK,
    FACTORY,
    FOREST,
    HOSPITAL,
    HOTEL,
    POND,
    SCHOOL,
    TEMPLE,
    UPTOWN,
    PLACE_END,

    // 사냥
    CHICKEN,
    BAT,
    BOAR,
    WILD_DOG,
    WOLF,
    BEAR,

    CRAFTING // 제작
};

enum class MONSTER_TYPE
{
    MONSTER_BAT,
    MONSTER_BEAR,
    MONSTER_BOAR,
    MONSTER_CHICKEN,
    MONSTER_DOG,
    MONSTER_WOLF,
    END
};

struct tItem
{
    ITEM            ID;
    ITEM_GRADE      Grade;
    ITEM_TYPE       Type;
    vector<APPEAR>  Appear;
    int             AppearNum = 3; // 한 지역에서 나올 수 있는 최대 갯수
    int             MaxQuantity; // 최대 소지량(겹칠 수 있는 아이템의 수)
    wstring         TextureName; // UI에서 보여질 텍스쳐 이름
    wstring         ModelName; // 맵 위에서 보여질 모델 이름
    bool            bCombination; // 조합해서 만들어진 아이템인가?
    ITEM            Ingredient1ID; // 재료템 아이디 1
    ITEM            Ingredient2ID; // 재료템 아이디 2
    vector<ITEM>    BuildItem; // 상위 아이템 목록
    int             Bullet; // 총, 투사체 인 경우 장전 수
    float           ReloadTime; // 투사체인 경우 충전 시간(총은 제외됨)

    float           AttackPower= 0.f; // 공격력 증가
    float           AttackPowerPerLevel= 0.f; // 레벨당 공격력 증가
    float           AttackSpeed= 0.f; // 공속 증가
    float           AttackSpeedPerLevel= 0.f; // 레벨당 공속 증가
    float           CriticalStrikeChance= 0.f; // 치명타 확률
    float           CriticalStrikeDamage= 0.f; // 치명타 피해량
    float           LifeSteal= 0.f; // 생명력 흡수
    float           Omnisyphon= 0.f; // 모든 피해 흡혈
    float           ExtraBaseAttackDamage= 0.f; // 기공추
    float           ExtraBaseAttackDamagePerLevel= 0.f; // 레벨당 기공추
    float           ArmorPenetration= 0.f; // 방어 관통
    
    float           ExtraSkillDamage= 0.f; // 스공추
    float           ExtraSkillDamagePerLevel= 0.f; // 레벨당 스공추
    float           SkillAmplification= 0.f; // 스킬 증폭
    float           SkillAmplificationPerLevel= 0.f; // 레벨당 스킬 증폭
    float           CooldownReduction= 0.f; // 쿨다운 감소
    float           MaxSP= 0.f; // 최대 스테미나 증가
    float           SPGen= 0.f; // 스테미나 회복 추가(고정값)
    float           SPGen_Percent= 0.f; // 스테미나 회복 추가(퍼센트)

    float           Defense= 0.f; // 방어력 증가
    float           DefensePerLevel= 0.f; // 레벨당 방어력 증가
    float           MaxHP= 0.f; // 최대 HP 증가
    float           MaxHPPerLevel= 0.f; // 레벨당 최대 HP 증가
    float           HPGen= 0.f; // 생명력 회복 추가(고정값)
    float           HPGen_Percent= 0.f; // 생명력 회복 추가(퍼센트)
    float           DefenseFromBasicAttack= 0.f; // 기본 공격 피해 감소
    float           SkillDamageReduction= 0.f; // 스킬 피해 감소
    float           SkillDamageReduction_Percent= 0.f; // 스킬 피해 감소(퍼센트)
    float           CriticalDamageReduction= 0.f; // 치명타 피해 감소

    float           MovementSpeed= 0.f; // 이동 속도 증가
    float           OutOfCombatMovementSpeed= 0.f; // 비전투시 이동 속도 증가
    float           VisionRange= 0.f; // 시야 범위 증가

    float           Tenacity= 0.f; // 방해 효과 저항
    float           AttackRange= 0.f; // 공격 범위 증가
    float           CooldownReductionCap= 0.f; // 최대 쿨다운 감소
    float           HealingReceived= 0.f; // 받는 회복 증가
};

struct tBelongings
{
    ITEM  ID;
    int   count;
};

enum class ATTACK_TYPE
{
    BASE,
    SKILL
};

enum class UI_TYPE
{
    DEFAULT, //일반 그림이나 판넬 처럼 아무런 영향이 없는 UI
    PROFILE,
    WEAPON_ICON,
    SKILL_IMAGE,
    WEAPON_SKILL_IMAGE,
    SKILL_LEVEL,
    FONT_FIX,
    FONT_CHANGEABLE,
    BUTTON_SKILL,
    BUTTON_PASSIVE_SKILL,
    BUTTON_NONE_DAY, // 버튼이 아닌 낮밤 아이콘을 위한 것 -> 그냥 UIScript에 하려고 하니까 좀 안 맞아서 그냥 버튼에다가 만듬,
    BUTTON,
    ITEM_BOX, // 버튼은 아니고 아이템ui의 위치를 잡아줄 ui
    HP_BAR,
    PROFILE_CHAR, // 각각의 캐릭터에 대한 프로필
    END
};

enum class SKILL_TYPE
{
    Q,
    W,
    E,
    R,
    T
};

enum class CHARACTER_TYPE
{
    AYA,
    HYUNWOO,
    LIDAILIN,
    END
};

enum class EQUIP_TYPE
{
    WEAPON,
    CHEST,
    HEAD,
    ARM,
    LEG,
    ACCESSORY,
    END
};