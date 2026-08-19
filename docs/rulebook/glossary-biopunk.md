# Biopunk Glossary

Term-by-term mapping from *Cthulhu: Death May Die* (Season 3) to the biopunk vocabulary of
Project_C. Used to generate `rulebook-biopunk.md` from `rulebook.md`.

**Scope: names only.** No mechanic, number, turn structure, resolution order or win/loss
condition changes. If a rename would force a mechanical change, the rename is wrong.

## The setting

A vertical city buried under its own chemical industry. The air below kills slowly. A cartel
refines a mutagen sold as salvation: it grants inhuman strength and takes the body as payment.

At the bottom of the sump, a **living organism** colonised decades of chemical runoff and has
been growing there for generations, feeding on what the city throws away. It is not sediment.
It is alive, it is vast, and it does not register humans as parties to a negotiation.

The cartel does not want to destroy it. They want to **refine** it and use it, convinced it can
be steered. Sinkers go down to contaminate the Batch and kill the thing before it fully wakes.

## Naming constraints

- No third-party proper nouns: Zaun, Shimmer, Piltover, chem-baron, Hextech, Umbrella.
- No Cthulhu/DMD nomenclature: Elder One, Mythos, Elder Sign, Cthulhu, Azathoth, Tsathoggua.
- Nothing that reads as magic or the occult. This is chemistry and heavy industry.
- `Grip` (Sanity) **decreases** toward death, exactly like the original. A term that increased
  would force inverting direction across dozens of sentences — that is a rewrite, not a rename.

## How to read the tables

The **Sense** column controls how the substitution is applied:

| Marker | Meaning |
|---|---|
| — | Unambiguous. Replace every occurrence, any case. |
| `Cap` | Replace only capitalised occurrences — the lowercase word has an ordinary English sense in this manual and must be left alone. |

## Casing policy

**Mirror the source.** If the original wrote `investigators` in lowercase, the biopunk document
writes `sinkers` in lowercase; if it wrote `Investigator`, it writes `Sinker`. The right-hand
column below is the *display* form of each term, not an instruction to capitalise.

This matters more than it looks. The source manual is itself inconsistent — it lowercases
`investigator`, `monsters`, `stress`, `sanity` and `ritual` in body text while capitalising
`Discovery cards`, `Elder One`, `Safe space`, `Run` and `Attack`. Mirroring that inconsistency is
what keeps the two documents **diffable line by line**, which is the entire point of having a
verifiable transcription. Normalising the casing would be an improvement to the original, and
improving the original is out of scope.

Exception: terms whose form is inherently a proper name — `the Growth`, `THE MAW`, `THE HOLLOW`
and the Sinker names — keep their given form regardless of the source's casing.

## Do not rename

| What | Why |
|---|---|
| The `CREDITS` block's people | Real names of real people. `Mike McVey`, `Studio McVey` and every other credited person stay untouched, even though the Sinker table maps `Mike` to `Bram`. |
| Game vocabulary inside `CREDITS` | The exception to the exception: `Additional Episode Design` becomes `Additional Job Design`, because it refers to the game's Jobs, not to a person. |
| The HTML comment block on page 1 | Transcription apparatus, not part of the game. |
| Icon tokens (`[TENTACLE]`, `[ELDER-SIGN]`, …) | Identifiers, not prose. `[TENTACLE]` stays `[TENTACLE]` even though the word `Tentacle` becomes `Rot`. |

---

## Core terms

| Original | Biopunk | Sense | Note |
|---|---|---|---|
| Investigator | Sinker | — | The one who goes down. |
| Elder One | the Growth | — | The verb is in the name: it grows. Carries the 4-stage boss. |
| Elder God | the Growth | — | The flavour text calls the same being an "Elder God". Left untranslated it would break the internal reference: the Cooks would summon an Elder God while the boss is called the Growth. |
| mad | to slip | — | Adjective, in "drive investigators mad". The noun `madness` maps separately. |
| Cultist | Cook | — | Cooks the Batch. Greed, not faith. |
| Monster | Byproduct | — | Waste of the refining. Cold on purpose: it appears hundreds of times. |
| Enemy | Hostile | — | Umbrella term covering Cooks, Byproducts and the Growth. |
| Sanity | Grip | — | "Losing your grip" already means losing your mind. Decreases, as required. |
| Insanity Threshold | Slip | — | You lose Grip, you Slip. The pair explains itself. |
| Insanity | Slip | — | The state, and the card type. |
| madness | the Slip | — | "consumed by madness" -> "lost to the Slip". |
| Stress | Strain | — | `Fatigue` was rejected: Fatigue Tokens already exist in the Tsathoggua box. |
| Wound | Wound | — | Unchanged. Already secular and unambiguous. |
| Skill | Skill | — | Unchanged. |
| Ritual | the Batch | — | Two independent lenses landed here. "Contaminating the Batch." |
| Episode | Job | — | |
| Safe space | Clear Air | — | A space with no Hostiles is a space where you can breathe. |
| Rest | Breathe | `Cap` | **Only the action.** "the rest of the", "putting away the rest" stay untouched. |
| Relic | Rig | — | |
| Companion | Stray | — | |
| Condition | Affliction | `Cap` | **Only the card type.** 7 capitalised occurrences are the card type; 1 lowercase occurrence is ordinary English ("winning conditions", p20) and must not become "winning afflictions". `Symptom` was rejected as the target: the manual already uses "symptoms" for Slip card effects. |
| Item | Item | — | Unchanged. |
| Gate | Drain | — | Hostiles come up out of the drains. |
| Story Board | Plant Board | — | The plant itself. `Site` was rejected: the source manual already uses "site" ("Kilkenny was the site of one of the first witch trials"). |
| Investigator board | Sinker board | — | |

## Dice faces and symbols

| Original | Biopunk | Sense | Note |
|---|---|---|---|
| Success | Success | — | Unchanged. |
| Tentacle | Rot | — | One syllable, reads on a die face. Costs 1 Grip. |
| Elder Sign | Grit | — | Inert unless a Skill uses it. Pairs with Rot. |
| Blank | Blank | — | Unchanged. |
| Bonus die | Kick die | — | A kick is a hit of the stuff. Carries no Rot face. |
| Standard die | Standard die | — | Unchanged. |
| Elder One Summoning | Batch Tick | — | The symbol; three of them in the discard pile advance the Growth, so a tally mark is what it is. `Advance` was rejected: the source uses "advances" 11 times for exactly this action. Always appears as "Elder One Summoning symbol(s)". |
| Summoning track | Batch track | — | Never preceded by "Elder One" in the source, so the two never collide. |
| Progression token | Escalation token | — | |
| Fire | Fire | — | Unchanged. Chemical fire needs no rename. |

## Card types

| Original | Biopunk | Sense |
|---|---|---|
| Mythos card | Incident card | — |
| Mythos deck | Incident deck | — |
| Discovery card | Salvage card | — |
| Discovery deck | Salvage deck | — |
| Insanity card | Slip card | — |
| Episode card | Job card | — |
| Elder One Stage | Growth Stage | — |
| Minions card | Minions card | — |

## Skills

Five of the six are already secular and stay as they are. Only the arcane one has to move.

| Original | Biopunk | Sense | Note |
|---|---|---|---|
| Arcane Mastery | Chem Sense | — | The skill that turns inert Grit into results. |
| Brawling | Brawling | — | Unchanged. |
| Marksman | Marksman | — | Unchanged. |
| Stealth | Stealth | — | Unchanged. |
| Swiftness | Swiftness | — | Unchanged. |
| Toughness | Toughness | — | Unchanged. |

## The harder variant

`Unknown` is DMD nomenclature (it names the season). Deeper means harder in a vertical city.

| Original | Biopunk | Sense |
|---|---|---|
| Unknown Rules | Deep Rules | — |
| Unknown Dashboard | Deep Dashboard | — |
| Unknown Monster | Deep Byproduct | — |
| Unknown Mythos card | Deep Incident card | — |
| Unknown Relic | Deep Rig | — |

## Named entities

The two boss boxes. Each is a distinct apex organism the undercity named for itself.

| Original | Biopunk | Sense |
|---|---|---|
| Tsathoggua | THE MAW | — |
| Azathoth | THE HOLLOW | — |
| Cosmic | Hollow | — |
| Fatigue token | Fatigue token | — |

## Byproducts

| Original | Biopunk | Sense |
|---|---|---|
| Ghast | Husk | — |
| Serpent Man | Coilman | — |
| Gug | Lurcher | — |
| Gyaa-Yothn | Draybeast | — |
| Fisher from Outside | Angler | — |
| The Unnamable | the Nameless | — |
| Dhole | Borer | — |
| Dancer of Azathoth | Hollow Twitcher | — |
| Amorphous Scion | Hollow Slurry | — |

## Sinkers

The original roster is DMD's cast. Replaced one-for-one, keeping ordinary human first names —
these are people from the undercity, not codenames.

| Original | Biopunk | Sense |
|---|---|---|
| Stella | Wren | — |
| Sandra | Marisol | — |
| Agatha | Odile | — |
| Mike | Bram | — |
| Leon | Cas | — |
| Ruth | Nadia | — |
| Julien | Emre | — |
| Huikong | Jiwon | — |
| Sally | Tess | — |
| Peters | Okonkwo | — |

## Title — provisional

| Original | Biopunk | Sense |
|---|---|---|
| CTHULHU DEATH MAY DIE | THE SUMP | — |
| Cthulhu: Death May Die | The Sump | — |
| FEAR OF THE UNKNOWN | THE DEEP | — |

**`THE SUMP` is a working title, not a decision.** Naming the game is a separate call from
renaming its vocabulary, and it should be made with the concept in hand — the `/brainstorm`
never finished. It is here only so the document has something to print on page 1.
