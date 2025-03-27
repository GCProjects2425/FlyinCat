# Projet A Flyin Cat

## Auteurs

- **TESSIER Loris**
- **MUSITELLI Gaspar**
- **FAVEROT Benjamin**

---

## Contrôles

| Action                     | Clavier/Souris    | Manette (XBOX)                    |
| -------------------------- | ----------------- | --------------------------------- |
| **Tendre le lance-pierre** | `S`               | `Left Joystick` / `Right Trigger` |
| **Engager le tir**         | `Espace`          | `A`                               |
| **Tirer**                  | Relacher `Espace`  | Relacher `A`                       |
| **Viser/Direction**        | `Souris`          | `Right Joystick`                  |
| **Revenir au menu**        | `Échap`           | `Start Button (≡)`                |

---

## Scène de démarrage

- **Gameplay** : `Content/Scene/TestMap.umap`
- **Menu** : `Content/UI/Scene_MainMenu.umap`

---

## Codes Principaux

### C++ Classes

- `BallLauncher.cpp` : Gestion du lance-pierre
- `FlyinCatCharacter.cpp` : Gestion des balles

### Blueprints

- `Content/UI/W_MainMenu.uasset` : Interface du menu principal

---

## Fonctionnalités

- Mécanique de tir avec lance-pierre
- Physique Chaos
- Interface utilisateur pour le menu principal
- Gestion des entrées clavier/souris et manette + force feedback

---

