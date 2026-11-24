#ifndef __WORDBANK_H__
#define __WORDBANK_H__

// —————— headers ————————————————————————— //
#include <avr/pgmspace.h>
// —————— headers ————————————————————————— //



// —————— macro functions ————————————————— //
#define LENG(_x) sizeof(_x) / sizeof(_x[0])
// —————— macro functions ————————————————— //



// —————— structures ————————————————— //
struct WordPack {
  const char* bank;
  uint16_t total;
  uint8_t leng;
};
// —————— structures ————————————————— //



// —————— word lists —————————————————————— //
const char words5[][6] PROGMEM = {
  "APPLE", "BEACH", "CHIPS", "DREAM", "EARTH", "PLANT", "SMILE", 
  "CLOUD", "FLAME", "OCEAN", "RIVER", "STONE", "STORM", "TREES", 
  "WATER", "PHONE", "MOUSE", "BREAD", "CABLE", "CHAIR", "CRATE", 
  "GRASS", "GREEN", "SOLAR", "BLOOM", "FROST", "SHORE", "SPACE", 
  "PRINT", "STATE", "LABEL", "STORE", "FIRST", "DRINK", "TRUCK", 
  "CLOCK", "GLASS", "KNIFE", "LIGHT", "PAPER", "PLATE", "RADIO", 
  "BOARD", "BRUSH", "FLASK", "FRAME", "MOTOR", "TOWEL", "VALVE", 
  "LEMON", "MANGO", "PEACH", "SUGAR", "SPOON", "SPICE", "GRAPE", 
  "SAUCE", "PIZZA", "BERRY", "HONEY", "ROAST", "CREAM", "FLOUR", 
  "BREAK", "BUILD", "CATCH", "DRIVE", "GLIDE", "JUMPY", "LAUGH", 
  "MATCH", "REACH", "SHINE", "STAND", "THINK", "WRITE", "START", 
  "BRAVE", "CLEAN", "CLEAR", "FRESH", "GREAT", "HAPPY", "HEAVY", 
  "LIGHT", "QUICK", "SHARP", "SMART", "SWEET", "WHITE", "YOUNG", 
  "BLOCK", "BYTES", "FLASH", "LOGIC", "PANEL", "PIXEL", "POWER", 
  "PULSE", "RESET", "SHIFT", "SIGMA", "STACK", "TRACK", "WIRES", 
  "ALARM", "ANGEL", "ARROW", "AUDIO", "BADGE", "BRICK", "BRING", 
  "CANDY", "CHEST", "CHIEF", "COACH", "COAST", "COINS", "CROWN", 
  "DANCE", "DEPTH", "DIARY", "EXTRA", "FAIRY", "FEVER", "FIELD", 
  "FLUID", "FOCUS", "FORCE", "FRONT", "FRUIT", "GIANT", "GLOBE", 
  "GLOVE", "GRAND", "GUIDE", "HOTEL", "HOUSE", "IMAGE", "INDEX", 
  "INPUT", "JUICE", "LASER", "LUNAR", "MARCH", "MODEL", "MONEY", 
  "MONTH", "MUSIC", "NIGHT", "NOISE", "NORTH", "PUNCH", "ROUND", 
  "ROUTE", "SCALE", "SCENE", "SCORE", "SHARK", "SLEEP", "SMOKE", 
  "SOUTH", "SOUND", "TRAIN", "TWICE", "VOICE", "WATCH", "WHEEL", 
  "WORLD", "ZEBRA", "ACTOR", "MUGIN", "MUGEN", "HUGIN", "BURQA", 
  "NYMPH", "OUIJA", "VOZHD", "WAQFS", "IAMBI", "MACAW", "OKAPI", 
};
const char words6[][7] PROGMEM = {
  "CAMERA", "BOTTLE", "BUTTON", "CHARGE", "PENCIL", "HEATER", 
  "BANNER", "WANDER", "CRATER", "MIRROR", "MARKER", "MEMORY", 
  "POCKET", "POWDER", "RUBBER", "VIDEOS", "WALLET", "WINTER", 
  "ACTION", "ANIMAL", "ATTACK", "AUTUMN", "BARKED", "BASKET", 
  "BEAUTY", "BEAVER", "BEFORE", "BELONG", "PHOTOS", "YELLOW",
  "BRANCH", "BRIDGE", "BRIGHT", "CASTLE", "CATCHS", "CHANCE", 
  "CHURCH", "CIRCLE", "CLEVER", "COFFEE", "CORNER", "NOBODY",
  "CREDIT", "DANGER", "DECIDE", "DEGREE", "DOCTOR", "DOLLAR", 
  "DRAGON", "DRIVER", "ENGINE", "ESCAPE", "EXPERT", "CONFIG",
  "FACTOR", "FAMILY", "FARMER", "FINGER", "FLIGHT", "FLOWER", 
  "FOREST", "FREEZE", "GARDEN", "GLANCE", "GUITAR", "LAUNCH",
  "HAMMER", "HANDLE", "HARBOR", "HEAVEN", "HOLLOW", "HUNTER", 
  "INSECT", "ISLAND", "JACKET", "SOCKET", "JUNGLE", "DIVIDE",
  "KITTEN", "NORDIC", "LEADER", "LIQUID", "LIZARD", "MONKEY", 
  "THANOS", "MOTHER", "NATURE", "NUMBER", "OFFICE", "GREASY",
  "ORANGE", "OXYGEN", "PACKET", "PALACE", "PERSON", "PLANET", 
  "POCKET", "POWDER", "PRINCE", "PRISON", "PUZZLE", "COMBAT",
  "WIGGLE", "RABBIT", "ROCKET", "RUBBER", "SADDLE", "SAFETY", 
  "SAILOR", "SAMPLE", "SCREEN", "SHADOW", "SILVER", "ARTIST",
  "SENSOR", "TYRANT", "PARROT", "WALRUS", "WHEELS", "VISION", 
  "OBJECT", "LOCATE", "GALAXY", "SECOND", "SONDER", "SERVER",
  "FUDGEL", "WORDLE", "BLITHE", "ACUMEN", "ZEALOT", "MIASMA", 
  "CHRISM", "NIMBLE", "SCHIST", "UMLAUT", "COCCYX", "ATTACK",
  "UPLOAD", "ALMOST", "SALUTE", "ONLINE", "SHAKER", "SEASON",
  "SENATE", "HUNGER", "SOCIAL", "BATMAN", "BURNER", "CANDLE",
  "CANYON", "CARTON", "GAMING",
};
const char words7[][8] PROGMEM = {
  "PICTURE", "MONITOR", "PITCHER", "LABELER", "SANCTUS", 
  "COASTER", "POPCORN", "CHEETAH", "LEOPARD", "QUARTET",
  "BATTERY", "CHASSIS", "PROJECT", "PLASTIC", "HANDSAW", 
  "PRINTER", "SPRAYER", "QUALIFY", "QUALITY", "SURVIVE",
  "ACADEMY", "ACCOUNT", "ACIDITY", "ADDRESS", "AGAINST", 
  "ALCOHOL", "ALREADY", "ANALYZE", "ANCIENT", "THOUGHT",
  "ANXIETY", "ARRANGE", "ARTICLE", "ATTEMPT", "ATTRACT", 
  "BALANCE", "BARGAIN", "BEDROOM", "BENEFIT", "SERIOUS",
  "BLANKET", "BRAVERY", "BREATHE", "BUILDER", "CABINET", 
  "CALCIUM", "CAPTAIN", "CAPSULE", "CARAVAN", "ORBITER",
  "CENTURY", "CHAMBER", "CHANNEL", "CHAPTER", "CHICKEN", 
  "CIRCUIT", "COLLEGE", "COMFORT", "COMMAND", "DUNGEON",
  "COMPANY", "CONCERT", "CONTROL", "COOKING", "COUNTER", 
  "COUNTRY", "CRITICS", "CULTURE", "CURRENT", "DRAWING",
  "CURTAIN", "DIAMOND", "DIGITAL", "DISCUSS", "DISEASE", 
  "DISPLAY", "DIVORCE", "DYNAMIC", "EASTERN", "VILLAIN",
  "ECONOMY", "ELEMENT", "EMERALD", "EMPEROR", "EVENING", 
  "EXACTLY", "EXAMPLE", "EXCITED", "EXCLUDE", "ANIMALS",
  "FACULTY", "FAILURE", "FANTASY", "FASHION", "FEATURE", 
  "FEDERAL", "FESTIVE", "FICTION", "FIFTEEN", "SMALLER",
  "FINALLY", "FINANCE", "FOREVER", "FORWARD", "FREEDOM", 
  "GALLERY", "GARMENT", "GATEWAY", "GENERAL", "RAILGUN",
  "GLACIER", "LAUGHED", "GOGGLES", "GORILLA", "GRAVITY", 
  "GROCERY", "HALFWAY", "HAMMOCK", "HARVEST", "LOUDEST",
  "BRUSQUE", "ERUDITE", "ASCETIC", "DEBACLE", "ALPHARD", 
  "PERFECT", "DIVIDED", "SEQUOIA", "SORGHUM", "SANDMAN"
};
// —————— word lists —————————————————————— //



// —————— word packs —————————————————————— //
const WordPack word_packs[3] PROGMEM = {
  {.bank = (const char*)words5, .total = LENG(words5), .leng = 5}, 
  {.bank = (const char*)words6, .total = LENG(words6), .leng = 6}, 
  {.bank = (const char*)words7, .total = LENG(words7), .leng = 7}, 
};
// —————— word packs —————————————————————— //

#endif