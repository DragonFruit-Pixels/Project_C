# D&D Dark Fantasy Glossary

Term-by-term mapping from *Cthulhu: Death May Die* (Season 3) to a dark medieval fantasy
vocabulary in the D&D register. Used to generate `rulebook-dnd.md` from `rulebook.md`.

**Scope: names only.** No mechanic, number, turn structure, resolution order or win/loss
condition changes. If a rename would force a mechanical change, the rename is wrong.

## The setting

Beneath a dead city something enormous lies **chained and leaking**. It was not killed; it was
bound, ages ago, and its ichor has been seeping into the vault ever since.

A cohort of **Binders** is completing **the Working**. They do not worship the thing — they mean
to bind it to their will, and they are certain it can be steered. If the Working closes, it wakes
whole, and owned.

The Sworn go down to ruin the Working and kill the thing while that is still possible. And the
ichor is right there, pooled on the floor, and **taking it helps**.

## The ratchet

Each draught of ichor grants the Sleeper's gifts and replaces what it displaces. `Marrow` only
falls, and resting does not restore it — the marrow is already gone. At the bottom of the track
the Sworn is not killed: they are **lost to the Brood** and pass into the enemy pool.

This is the *Baldur's Gate 3* parasite, with one improvement: there, the tadpole and the villain
are merely linked. Here the power you take **is** the boss. Every gift is a piece of the thing
you came down to kill.

## Naming constraints

- No Cthulhu/DMD nomenclature: Elder One, Mythos, Elder Sign, Cthulhu, Azathoth, Tsathoggua.
- No word that already appears in the source manual. Measured: 0 of 22 core terms reuse one.
- **No Wizards Product Identity creatures.** Beholder and mind flayer/illithid are excluded from
  SRD 5.1 — only their *names* appear in the document, with no stat block, description or art.
  The eye-horror in this set occupies that fantasy slot but is designed, not copied. The BG3
  parasite is likewise a trope and a mechanic, which are not protectable; the illithid is not.
- Creatures drawn either from SRD 5.1 (CC-BY-4.0) or from public-domain myth. Per-creature
  provenance is recorded in the Brood table below.
- `Marrow` (Sanity) **decreases** toward death, exactly like the original.

## How to read the tables

| Marker | Meaning |
|---|---|
| — | Unambiguous. Replace every occurrence, any case. |
| `Cap` | Replace only capitalised occurrences — the lowercase word has an ordinary English sense in this manual and must be left alone. |

## Casing policy

**Mirror the source.** If the original wrote `investigators` lowercase, this document writes
`sworn` lowercase; if it wrote `Investigator`, it writes `Sworn`. The right-hand column is each
term's *display* form, not an instruction to capitalise. Mirroring the source's own inconsistency
is what keeps the documents **diffable line by line**, which is the point of a verifiable
transcription.

Exception: inherently proper names — `KHARZUL`, `THE HUNDRED EYES` and the Sworn names — keep
their given form.

## Do not rename

| What | Why |
|---|---|
| The `CREDITS` block's people | Real names. `Mike McVey`, `Studio McVey`, `Richard Wright` and every other credited person stay untouched, even where the Sworn table maps `Mike`. |
| Game vocabulary inside `CREDITS` | The exception to the exception: `Additional Episode Design` becomes `Additional Vault Design`. |
| The HTML comment block on page 1 | Transcription apparatus, not part of the game. |
| Icon tokens (`[TENTACLE]`, `[ELDER-SIGN]`, …) | Identifiers, not prose. `[TENTACLE]` stays `[TENTACLE]` even though `Tentacle` becomes `Blight`. |

---

## Core terms

| Original | D&D | Sense | Note |
|---|---|---|---|
| Investigator | Sworn | — | They swore to go down and to take the ichor. |
| Elder One | the Sleeper | — | Generic slot term covering both boss boxes. Pairs with the Waking track: "the Sleeper advances along the Waking track" explains itself, and explains why it cannot be attacked yet — it is not fully here. |
| Elder God | the Sleeper | — | The flavour text calls the same being an "Elder God". |
| Cultist | Binder | — | They mean to bind it, not to serve it. |
| Monster | Brood | — | Everything that grew around a Sleeper. Cold and short: it appears hundreds of times. |
| Enemy | Foe | — | Umbrella term covering Binders, Brood and the Sleeper. |
| Sanity | Marrow | — | Displaced by ichor. Falls only, and resting cannot restore it. |
| Insanity Threshold | Bloom | — | The ichor blooms in you: the gift and the cost arrive together. |
| Insanity | Bloom | — | The state, and the card type. |
| madness | the Brood | — | "consumed by madness" -> "lost to the Brood". |
| mad | to bloom | — | Adjective, in "drive investigators mad". Shown lowercase on purpose: the only source occurrence is lowercase, and a target beginning with a function word ("to") defeats automatic case-mirroring, so the display form has to carry the right case itself. |
| Stress | Sap | — | Spent to reroll, recovered by Binding. Sapped, not emptied. |
| Wound | Wound | — | Unchanged. Already secular and unambiguous. |
| Skill | Skill | — | Unchanged. |
| Ritual | the Working | — | Genuine occult term for a magical operation. "Ruining the Working." |
| Episode | Vault | — | Each scenario is a vault under the city. |
| Safe space | Warded | — | Uses D&D's own logic: "a space is Warded when there are no Foes in it" needs no glossary. |
| Rest | Bind | `Cap` | **Only the action** (bind your wounds). "the rest of the", "putting away the rest" stay untouched. |
| Relic | Talisman | — | |
| Companion | Hireling | — | A D&D term for exactly this: expendable, absorbs what you cannot. |
| Condition | Curse | `Cap` | **Only the card type.** The lowercase "winning conditions" (p20) is ordinary English and must not become "winning curses". |
| Item | Item | — | Unchanged. |
| Gate | Rift | — | |
| Story Board | Vault Board | — | |
| Investigator board | Sworn board | — | |

## Dice faces and symbols

| Original | D&D | Sense | Note |
|---|---|---|---|
| Success | Success | — | Unchanged. |
| Tentacle | Blight | — | Costs 1 Marrow. One syllable, reads on a die face. |
| Elder Sign | Glyph | — | Inert unless a Skill can read it. |
| Blank | Blank | — | Unchanged. |
| Bonus die | Boon die | — | Direct D&D terminology (boon/bane). Carries no Blight face. |
| Standard die | Standard die | — | Unchanged. |
| Elder One Summoning | Waking | — | The symbol. Always appears as "Elder One Summoning symbol(s)". |
| Summoning track | Waking track | — | Never preceded by "Elder One" in the source, so the two never collide. |
| Progression token | Rousing token | — | |
| Fire | Fire | — | **Unchanged, and now earned.** In the original, Fire tokens are unexplained. With an ancient red in the first boss box, Fire is the dragon's breath. |

## Card types

| Original | D&D | Sense |
|---|---|---|
| Mythos card | Portent card | — |
| Mythos deck | Portent deck | — |
| Discovery card | Hoard card | — |
| Discovery deck | Hoard deck | — |
| Insanity card | Bloom card | — |
| Episode card | Vault card | — |
| Elder One Stage | Sleeper Stage | — |
| Minions card | Minions card | — |

## Skills

Five of the six are generic English rather than DMD nomenclature, so they stay. Only the arcane
one moves — and the replacement is an improvement, because it names what the skill does to the
die face.

| Original | D&D | Sense | Note |
|---|---|---|---|
| Arcane Mastery | Glyphwork | — | The skill that turns inert Glyphs into successes. |
| Brawling | Brawling | — | Unchanged. |
| Marksman | Marksman | — | Unchanged. |
| Stealth | Stealth | — | Unchanged. |
| Swiftness | Swiftness | — | Unchanged. |
| Toughness | Toughness | — | Unchanged. |

## The harder variant

`Unknown` is DMD nomenclature — it names the season. Deeper vaults are worse vaults.

| Original | D&D | Sense |
|---|---|---|
| Unknown Rules | Sunken Rules | — |
| Unknown Dashboard | Sunken Dashboard | — |
| Unknown Monster | Sunken Brood | — |
| Unknown Mythos card | Sunken Portent card | — |
| Unknown Relic | Sunken Talisman | — |

## The two Sleepers

Two boss boxes, deliberately different in texture, as in the original: one is a physical
apex predator, the other is a wrongness.

| Original | D&D | Sense |
|---|---|---|
| Tsathoggua | KHARZUL | — |
| Azathoth | THE HUNDRED EYES | — |
| Cosmic | Gaze | — |
| Fatigue token | Cinder token | — |

**KHARZUL, THE RED** — an ancient red dragon, chained under the city and leaking. Its four Stages
are a dragon's age categories: wyrmling, young, adult, ancient. In DMD the Stages accumulate dice
and effects as they are revealed, which is exactly what a dragon does as it ages, so the boss's
strangest rule stops needing an explanation. Its Fire tokens are its breath.

**THE HUNDRED EYES** — an aberration that empties what it looks at. It occupies the slot a
beholder would fill and is deliberately **not** one: that creature is Wizards Product Identity
with no SRD stat block to work from. Its minions are Watchers, and the Husks it has finished
looking at.

## Brood

Per-creature provenance, because it decides what can ship commercially.

| Original | D&D | Sense | Provenance |
|---|---|---|---|
| Ghast | Ghoul | — | Folklore + SRD 5.1 |
| Serpent Man | Kobold | — | SRD 5.1. In D&D lore kobolds serve dragons, so the basic minion and KHARZUL are connected without a line of lore |
| Gug | Cyclops | — | Homer. Public domain |
| Gyaa-Yothn | Basilisk | — | Myth + SRD 5.1 |
| Fisher from Outside | Harpy | — | Myth + SRD 5.1 |
| The Unnamable | Chimera | — | Myth + SRD 5.1 |
| Dhole | Purple Worm | — | SRD 5.1. Already a giant burrowing worm |
| Dancer of Azathoth | Watcher | — | Original |
| Amorphous Scion | Husk | — | Original |

## Sworn

The original roster is DMD's cast. Replaced one for one, keeping ordinary human names — these
are people from the city above, not codenames.

| Original | D&D | Sense |
|---|---|---|
| Stella | Ysolde | — |
| Sandra | Maren | — |
| Agatha | Bertrude | — |
| Mike | Corvin | — |
| Leon | Aldric | — |
| Ruth | Hedda | — |
| Julien | Emeric | — |
| Huikong | Shulan | — |
| Sally | Nessa | — |
| Peters | Volker | — |

## Title — provisional

| Original | D&D | Sense |
|---|---|---|
| CTHULHU DEATH MAY DIE | THE WAKING | — |
| Cthulhu: Death May Die | The Waking | — |
| FEAR OF THE UNKNOWN | THE SUNKEN | — |

**`THE WAKING` is a working title, not a decision.** Naming the game is a separate call from
renaming its vocabulary. It is here only so the document has something to print on page 1.
