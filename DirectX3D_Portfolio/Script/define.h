#pragma once

enum class ITEM
{
    // �Ϲ� ���
    // �ܰ�
    SCISSORS,
    FOUNTAIN_PEN,
    KITCHEN_KNIFE,

    // ��հ�
    RUSTY_SWORD,

    // ����
    PICKAXE,
    HARCHET,

    // �ְ�
    TWIN_BLADES,

    // �����
    BRANCH,
    SHORT_ROD,

    //��ġ
    HAMMER,

    //�۷���
    BRASS_KNUCKLES,
    COTTON_GLOVES,

    //����
    BAMBOO,

    // â
    SHORT_SPEAR,

    // ����
    WALTHER_PPK,

    // ������
    FEDOROVA,

    // ������
    LONG_RIFLE,

    // ������
    STEEL_CHAIN,

    // ��Ÿ ����
    IRON_BALL,
    RAZOR,
    PLAYING_CARD,
    CHALK,
    SHORT_CROSSBOW,
    NEEDLE,

    // ����
    WINDBREAKER,
    MONKS_ROBE,
    WETSUIT,
    FABRIC_ARMOR,

    // �Ӹ�
    HAIRBAND,
    HAT,
    BIKE_HELMET,

    // ��
    WATCH,
    BANDAGE,
    BRACELET,

    // �ٸ�
    SLIPPERS,
    RUNNING_SHOES,
    TIGHTS,

    // ��ű�
    FEATHER,
    FLOWER,
    RIBBON,
    FAN,
    BUDDHIST_SCRIPTURE,
    BOX,
    HOLY_GRAIL,
    CROSS,
    BINOCULARS,

    // ����
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

    // ����
    HONEY,
    WATER,
    ICE,
    WHISKEY,
    COFFEE,
    CARBONATED_WATER,
    MILK,

    // ���
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

    // ��� ���
    // �ܰ�
    JAMADHAR,
    MILITARY_KNIFE,
    SCALPEL,

    // ��հ�
    SHAMSHIR,
    PLASMA_SWORD,

    // �ְ�
    WROUGHT_SWORDS,

    //�۷���
    LEATHER_GLOVES,
    IRON_KNUCKLES,

    //����
    WOODEN_TONFA,

    // â
    BAMBOO_SPEAR,

    // ����
    MAGNUM_PYTHON,
    BERETTA_M92F,

    // ������
    STG_44,

    // ������
    SPRINGFIELD,

    // ������
    NUNCHAKU,

    // ����
    LEATHER_ARMOR,
    LEATHER_JACKET,
    TURTLE_DOBOK,
    MILITARY_SUIT,
    PATCHED_ROBE,
    DRESS,
    BIKINI,
    DIVING_SUIT,
    DEACON_ROBES,

    // �Ӹ�
    MASK,
    CIRCLET,
    BERET,
    CHAIN_COIF,
    SAFETY_HELMET,

    // ��
    LEATHER_SHIELD,
    SQUAD_LEATHER_ARMBAND,
    BRACER,
    BROKEN_WATCH,

    // �ٸ�
    KNEE_PADS,
    CHAIN_LEGGINGS,
    HIGH_HEELS,
    HEELYS,
    REPAIRED_SLIPPERS,
    BOOTS,

    // ��ű�
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

    // ����
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

    // ����
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

    // ���
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

    // ��� ���
    // �ܰ�
    ROSE_KNIFE,
    SWISS_ARMY_KNIFE,
    CARAPACE_KATAR,

    // ��հ�
    KATANA,
    MASAMUNE,
    MURAMASA,
    BASTARD_SWORD,
    JEWEL_SWORD,

    // �ְ�
    FLORENTINE,
    POCKET_ACES,

    //�۷���
    GAUNTLET,
    WING_KNUCKLES,
    BONE_GAUNTLET,
    SHATTER_SHALL_GAUNTLET,
    GLASS_KNUCKLES,
    PHOENIX_GLOVES,

    //����
    POLICE_BATON,
    RYUKYU_TONFA,

    // â
    BIDENT,
    PIKE,
    HALBERD,
    SHARPENED_SPEAR,

    // ����
    FN57,
    DOUBLE_REVOLVER_SP,
    MAGNUM_ANACONDA,

    // ������
    AK_47,
    M16A1,

    // ������
    HARPOON_GUN,
    GOLDEN_RIFLE,
    RAILGUN,

    // ������
    SHARPER,
    BLEEDER,

    // ����
    RIDER_JACKET,
    CHAIN_ARMOR,
    SUIT,
    QIPAO,
    SHEET_METAL_ARMOR,
    HANBOK,
    CARDINAL_ROBES,
    BULLETPROOF_VEST,
    COVERT_AGENT_UNIFORM,

    // �Ӹ�
    BALLISTIC_HELMET,
    FIRE_HELMET,
    TIARA,
    CROWN,
    CLOSE_HELM,
    MOTORCYCLE_HELMET,

    // ��
    SHEATH,
    GOLDEN_BRACELET,
    BAZUBAND,
    BARBED_BLOSSOM,
    STEEL_SHIELD,

    // �ٸ�
    GLACIER_CRAMPONS,
    STEEL_KNEE_PADS,
    COMBAT_BOOTS,
    KILLER_HEELS,

    // ��ű�
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

    // ����
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

    // ����
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

    // ���
    TREE_OF_LIFE,
    METEORITE,
    MOONSTONE,
    POISON,
    MOTOR,
    MITHRIL,
    GLASS_PANEL,
    ION_BATTERY,
    CELL_PHONE,

    // ���� ���
    // �ܰ�
    CARNWENNAN,
    MOUNTAIN_SLICER,
    VIBROBLADE,
    MAHARAJA,

    // ��հ�
    THUAN_THIEN,
    ARONDIGHT,
    EXCALIBUR,
    MONOHOSHIZAO,
    HOVUD,

    // �ְ�
    STARSTEEL_TWIN_SWORDS,
    ASURA,
    DIOSCURI,

    //�۷���
    ONE_INCH_PUNCH,
    DIVINE_FIST,
    BLOODWING_KNUCKLES,
    FROST_PETAL_HAND,
    BUDDHAS_PALM,
    BRASIL_GAUNTLET,
    WHITE_CLAW_PUNCH,

    //����
    TATICAL_TONFA,
    MAI_SOK,
    PLASMA_TONFA,

    // â
    GENTIAN_SILVER_GUN,
    EIGHTEEN_FOOT_SPEAR,
    COSMIC_BIDENT,
    LANCE_OF_POSEIDON,
    FANGTIAN_HUAJI,
    DRAGON_GUANDAO,

    // ����
    DEVILS_MARKSMAN,
    ELEGANCE,
    ELECTRON_BLASTER,
    MAGNUM_BOA,

    // ������
    GATLING_GUN,
    TYPE_95,
    AK_12,
    XCR,

    // ������
    TAC_50,
    NTW_20,
    POLARIS,

    // ������
    THE_SMITING_DRAGON,
    VIBRO_NUNCHAKU,
    CERBERUS,

    // ����
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

    // �Ӹ�
    MITHRIL_HELM,
    CRYSTAL_TIARA,
    TACTICAL_OPS_HELMET,
    HELM_OF_BANNERET,
    IMPERIAL_CROWN,
    IMPERIAL_BURGONET,
    DIADEM,
    
    // ��
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
    
    // �ٸ�
    FEATHER_BOOTS,
    MAVERICK_RUNNER,
    STRAITJACKET_SNEAKERS,
    MITHRIL_BOOTS,
    BUCEPHALUS,
    WHITE_RHINOS,
    TACHYON_BRACE,
    IRON_MAIDEN,
    
    // ��ű�
    MOONLIGHT_PENDANT,
    GLACIAL_ICE,
    TRUE_SAMADHI_FIRE,
    LUNAR_EMBRACE,
    MITHIRIL_QUIVER,
    
    // ����
    ZEN_VITALITY,
    FISH_AND_CHIPS,
    
    // ���
    VF_BLOOD_SAMPLE,
    FORCE_CORE,

    // ���� ���
    // �ܰ�
    FRAGARACH,

    // ��հ�
    LAEVATEINN,
    AURORA_LONGSWORD,
    DAINSLEIF,

    // �ְ�
    LLOIGOR_N_ZAHR,
    SPRING_AND_AUTUMN,

    //�۷���
    MARK_OF_THE_PHOENIX,
    PERMAFROST,

    //����
    OBSIDIAN_JITTE,

    // â
    BLAZING_LANCE,
    SPEAR_OF_LONGINUS,

    // ����
    KELTE,

    // ������
    JUDGEMENT,
    AGNI,

    // ������
    THE_DEADLY_RAY,
    BLACKFIRE_CANNON,

    // ������
    HYDRA,

    // ����
    BLAZING_DRESS,
    KABANA,
    QUEEN_OF_HEARTS,
    HOLY_ORDERS,
    BURGUNDY_47,

    // �Ӹ�
    LAUREL_WREATH,
    CHINESE_OPERA_MASK,
    ELYSIAN_HALO,
    FALLEN_PEGASUS,
    
    // ��
    TELLURIAN_TIMEPIECE,
    RADER,
    AUTO_ARMS,
    
    // �ٸ�
    EOD_BOOTS,
    GLACIAL_SHOES,
    BOOTS_OF_HERMES,
    RED_SHOES,
    
    // ��ű�
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
    // ���� ��ġ
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

    // ���
    CHICKEN,
    BAT,
    BOAR,
    WILD_DOG,
    WOLF,
    BEAR,

    CRAFTING // ����
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
    int             AppearNum = 3; // �� �������� ���� �� �ִ� �ִ� ����
    int             MaxQuantity; // �ִ� ������(��ĥ �� �ִ� �������� ��)
    wstring         TextureName; // UI���� ������ �ؽ��� �̸�
    wstring         ModelName; // �� ������ ������ �� �̸�
    bool            bCombination; // �����ؼ� ������� �������ΰ�?
    ITEM            Ingredient1ID; // ����� ���̵� 1
    ITEM            Ingredient2ID; // ����� ���̵� 2
    vector<ITEM>    BuildItem; // ���� ������ ���
    int             Bullet; // ��, ����ü �� ��� ���� ��
    float           ReloadTime; // ����ü�� ��� ���� �ð�(���� ���ܵ�)

    float           AttackPower= 0.f; // ���ݷ� ����
    float           AttackPowerPerLevel= 0.f; // ������ ���ݷ� ����
    float           AttackSpeed= 0.f; // ���� ����
    float           AttackSpeedPerLevel= 0.f; // ������ ���� ����
    float           CriticalStrikeChance= 0.f; // ġ��Ÿ Ȯ��
    float           CriticalStrikeDamage= 0.f; // ġ��Ÿ ���ط�
    float           LifeSteal= 0.f; // ����� ���
    float           Omnisyphon= 0.f; // ��� ���� ����
    float           ExtraBaseAttackDamage= 0.f; // �����
    float           ExtraBaseAttackDamagePerLevel= 0.f; // ������ �����
    float           ArmorPenetration= 0.f; // ��� ����
    
    float           ExtraSkillDamage= 0.f; // ������
    float           ExtraSkillDamagePerLevel= 0.f; // ������ ������
    float           SkillAmplification= 0.f; // ��ų ����
    float           SkillAmplificationPerLevel= 0.f; // ������ ��ų ����
    float           CooldownReduction= 0.f; // ��ٿ� ����
    float           MaxSP= 0.f; // �ִ� ���׹̳� ����
    float           SPGen= 0.f; // ���׹̳� ȸ�� �߰�(������)
    float           SPGen_Percent= 0.f; // ���׹̳� ȸ�� �߰�(�ۼ�Ʈ)

    float           Defense= 0.f; // ���� ����
    float           DefensePerLevel= 0.f; // ������ ���� ����
    float           MaxHP= 0.f; // �ִ� HP ����
    float           MaxHPPerLevel= 0.f; // ������ �ִ� HP ����
    float           HPGen= 0.f; // ����� ȸ�� �߰�(������)
    float           HPGen_Percent= 0.f; // ����� ȸ�� �߰�(�ۼ�Ʈ)
    float           DefenseFromBasicAttack= 0.f; // �⺻ ���� ���� ����
    float           SkillDamageReduction= 0.f; // ��ų ���� ����
    float           SkillDamageReduction_Percent= 0.f; // ��ų ���� ����(�ۼ�Ʈ)
    float           CriticalDamageReduction= 0.f; // ġ��Ÿ ���� ����

    float           MovementSpeed= 0.f; // �̵� �ӵ� ����
    float           OutOfCombatMovementSpeed= 0.f; // �������� �̵� �ӵ� ����
    float           VisionRange= 0.f; // �þ� ���� ����

    float           Tenacity= 0.f; // ���� ȿ�� ����
    float           AttackRange= 0.f; // ���� ���� ����
    float           CooldownReductionCap= 0.f; // �ִ� ��ٿ� ����
    float           HealingReceived= 0.f; // �޴� ȸ�� ����
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
    DEFAULT, //�Ϲ� �׸��̳� �ǳ� ó�� �ƹ��� ������ ���� UI
    PROFILE,
    WEAPON_ICON,
    SKILL_IMAGE,
    WEAPON_SKILL_IMAGE,
    SKILL_LEVEL,
    FONT_FIX,
    FONT_CHANGEABLE,
    BUTTON_SKILL,
    BUTTON_PASSIVE_SKILL,
    BUTTON_NONE_DAY, // ��ư�� �ƴ� ���� �������� ���� �� -> �׳� UIScript�� �Ϸ��� �ϴϱ� �� �� �¾Ƽ� �׳� ��ư���ٰ� ����,
    BUTTON,
    ITEM_BOX, // ��ư�� �ƴϰ� ������ui�� ��ġ�� ����� ui
    HP_BAR,
    PROFILE_CHAR, // ������ ĳ���Ϳ� ���� ������
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