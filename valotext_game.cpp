#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <windows.h> // For changing console colors

using namespace std;

struct Agent {
    string name;
    string ability;
    int health;
    bool abilityUsed;
    int abilityCooldown; // Track the cooldown of the ability

    Agent(string n, string a, int h) : name(n), ability(a), health(h), abilityUsed(false), abilityCooldown(0) {}
};

// Function to simulate an attack
int attack(int& health, int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
    return health;
}

// Function to display text with delay for better readability
void displayText(const string& text, int delay = 50) {
    for (char ch : text) {
        cout << ch;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    cout << endl;
}

// Function to clear the console
void clearScreen() {
    system("CLS");
}

// Function to set console text and background color
void setConsoleColours(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

// Special abilities for agents
void jettAbility(int& enemyHealth, Agent& agent) {
    if (agent.abilityCooldown == 0) {
        displayText(agent.name + " uses Blade Storm! Her precision strikes leave the enemy vulnerable.");
        int damage = 40;
        enemyHealth = attack(enemyHealth, damage);
        displayText("Blade Storm inflicted " + to_string(damage) + " damage on the enemy.");
        agent.abilityCooldown = 3; // Set a cooldown of 3 turns
    }
    else {
        displayText(agent.name + "'s ability is on cooldown for " + to_string(agent.abilityCooldown) + " more turns.");
    }
}

void phoenixAbility(Agent& player, int& enemyHealth) {
    if (player.abilityCooldown == 0) {
        displayText(player.name + " activates Run It Back! His flames engulf him, restoring his vitality.");
        int heal = 30;
        player.health += heal;
        if (player.health > 100) player.health = 100;
        enemyHealth = attack(enemyHealth, 20);
        displayText("Phoenix regained " + to_string(heal) + " health and dealt 20 damage to the enemy.");
        player.abilityCooldown = 3; // Set a cooldown of 3 turns
    }
    else {
        displayText(player.name + "'s ability is on cooldown for " + to_string(player.abilityCooldown) + " more turns.");
    }
}

void sageAbility(Agent& player, vector<Agent>& team) {
    if (player.abilityCooldown == 0) {
        displayText(player.name + " casts Healing Orb! Vitality surges through her team.");
        for (auto& member : team) {
            if (member.health > 0) {
                member.health += 30;
                if (member.health > 100) member.health = 100;
                displayText(member.name + " restored health to " + to_string(member.health) + ".");
            }
        }
        player.abilityCooldown = 3; // Set a cooldown of 3 turns
    }
    else {
        displayText(player.name + "'s ability is on cooldown for " + to_string(player.abilityCooldown) + " more turns.");
    }
}

void brimstoneAbility(int& enemyHealth, Agent& agent) {
    if (agent.abilityCooldown == 0) {
        displayText(agent.name + " calls in an Orbital Strike! A searing beam devastates the enemy.");
        int damage = 60;
        enemyHealth = attack(enemyHealth, damage);
        displayText("Orbital Strike inflicted " + to_string(damage) + " damage on the enemy.");
        agent.abilityCooldown = 3; // Set a cooldown of 3 turns
    }
    else {
        displayText(agent.name + "'s ability is on cooldown for " + to_string(agent.abilityCooldown) + " more turns.");
    }
}

void killjoyAbility(int& enemy1Health, int& enemy2Health, Agent& agent, bool& enemiesImmobilized) {
    if (agent.abilityCooldown == 0) {
        displayText(agent.name + " deploys Lockdown! Both enemies are immobilised and skip their next turn.");
        enemy1Health = attack(enemy1Health, 0); // No damage, just immobilise
        enemy2Health = attack(enemy2Health, 0); // No damage, just immobilise
        enemiesImmobilized = true;
        agent.abilityCooldown = 3; // Set a cooldown of 3 turns
    }
    else {
        displayText(agent.name + "'s ability is on cooldown for " + to_string(agent.abilityCooldown) + " more turns.");
    }
}

// Enemy attack targeting a random team member
void enemyAttack(vector<Agent>& team, int damage, string enemyName) {
    int targetIndex;
    do {
        targetIndex = rand() % team.size();
    } while (team[targetIndex].health <= 0);

    team[targetIndex].health = attack(team[targetIndex].health, damage);
    displayText(enemyName + " attacks " + team[targetIndex].name + ", dealing " + to_string(damage) + " damage.");
}

void updateCooldowns(vector<Agent>& team) {
    for (Agent& agent : team) {
        if (agent.abilityCooldown > 0) {
            agent.abilityCooldown--; // Decrease cooldown each turn
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Set console colours
    setConsoleColours(0, 4); // Black text on a red background

    // Agents and teams
    vector<Agent> agents = {
        Agent("Jett", "Blade Storm", 100),
        Agent("Phoenix", "Run It Back", 100),
        Agent("Sage", "Healing Orb", 100),
        Agent("Brimstone", "Orbital Strike", 100),
        Agent("Killjoy", "Lockdown", 100)
    };

    vector<Agent> team;

    // Available enemy names
    vector<string> enemyNames = {
        "TenZ", "Shroud", "Scream", "Derke", "Nivera", "Zellsis",
        "Yay", "Cned", "Sacy", "ShahZaM"
    };

    displayText("Welcome to the Valotext Game!\n");
    displayText("Choose 3 agents for your team:\n");

    // Select team members
    for (size_t i = 0; i < agents.size(); ++i) {
        displayText(to_string(i + 1) + ". " + agents[i].name + " - " + agents[i].ability);
    }

    for (int i = 0; i < 3; ++i) {
        displayText("Choose agent " + to_string(i + 1) + " by entering the corresponding number: ");
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice > 0 && choice <= agents.size()) {
            team.push_back(agents[choice - 1]);
        }
        else {
            displayText("Invalid choice. Defaulting to Jett.");
            team.push_back(agents[0]);
        }
    }

    // Let the player choose enemy names
    displayText("\nChoose names for the enemies from the following list:\n");
    for (size_t i = 0; i < enemyNames.size(); ++i) {
        displayText(to_string(i + 1) + ". " + enemyNames[i]);
    }

    string enemy1Name, enemy2Name;
    int enemyChoice1, enemyChoice2;

    // Player chooses names for Enemy 1 and Enemy 2
    displayText("Choose name for Enemy 1 (enter the corresponding number): ");
    cin >> enemyChoice1;
    cin.ignore();
    if (enemyChoice1 > 0 && enemyChoice1 <= enemyNames.size()) {
        enemy1Name = enemyNames[enemyChoice1 - 1];
    }
    else {
        displayText("Invalid choice. Defaulting to TenZ.");
        enemy1Name = enemyNames[0];
    }

    displayText("Choose name for Enemy 2 (enter the corresponding number): ");
    cin >> enemyChoice2;
    cin.ignore();
    if (enemyChoice2 > 0 && enemyChoice2 <= enemyNames.size()) {
        enemy2Name = enemyNames[enemyChoice2 - 1];
    }
    else {
        displayText("Invalid choice. Defaulting to Shroud.");
        enemy2Name = enemyNames[1];
    }

    // Set enemy healths
    int enemy1Health = 125;
    int enemy2Health = 125;
    bool enemiesImmobilized = false; // Track whether enemies are immobilised
    displayText("\nYour team is ready to face the enemies " + enemy1Name + " and " + enemy2Name + "!\n");

    // Game loop
    while (!team.empty() && (enemy1Health > 0 || enemy2Health > 0)) {
        clearScreen();
        displayText("Your turn! Choose an agent:\n");

        // List available agents
        for (size_t i = 0; i < team.size(); ++i) {
            if (team[i].health > 0) {
                displayText(to_string(i + 1) + ". " + team[i].name + " (Health: " + to_string(team[i].health) + ")");
            }
        }

        // Display enemies' health
        displayText("\nEnemies' health:\n");
        displayText(enemy1Name + ": " + to_string(enemy1Health));
        displayText(enemy2Name + ": " + to_string(enemy2Health));

        int agentChoice;
        displayText("Choose an agent by entering their number: ");
        cin >> agentChoice;
        cin.ignore();

        if (agentChoice > 0 && agentChoice <= team.size() && team[agentChoice - 1].health > 0) {
            Agent& activeAgent = team[agentChoice - 1];

            displayText("Choose action: 1 for Attack, 2 for Ability: ");
            int action;
            cin >> action;
            cin.ignore();

            if (action == 1) {
                // Choose target for attack
                displayText("Choose target: 1 for " + enemy1Name + ", 2 for " + enemy2Name + ": ");
                int target;
                cin >> target;
                cin.ignore();

                int damage = rand() % 20 + 10;
                if (target == 1) {
                    enemy1Health = attack(enemy1Health, damage);
                    displayText(activeAgent.name + " attacks " + enemy1Name + ", inflicting " + to_string(damage) + " damage.");
                }
                else if (target == 2) {
                    enemy2Health = attack(enemy2Health, damage);
                    displayText(activeAgent.name + " attacks " + enemy2Name + ", inflicting " + to_string(damage) + " damage.");
                }
                else {
                    displayText("Invalid target. Skipping turn.");
                }
            }
            else if (action == 2) {
                // Use ability
                if (activeAgent.name == "Jett") {
                    jettAbility(enemy1Health, activeAgent);
                }
                else if (activeAgent.name == "Phoenix") {
                    phoenixAbility(activeAgent, enemy1Health);
                }
                else if (activeAgent.name == "Sage") {
                    sageAbility(activeAgent, team);
                }
                else if (activeAgent.name == "Brimstone") {
                    brimstoneAbility(enemy1Health, activeAgent);
                }
                else if (activeAgent.name == "Killjoy") {
                    killjoyAbility(enemy1Health, enemy2Health, activeAgent, enemiesImmobilized);
                }
                else {
                    displayText("Ability not implemented yet!");
                }
            }
            else {
                displayText("Invalid action. Skipping turn.");
            }

            if (enemy1Health <= 0 && enemy2Health <= 0) {
                displayText("\nVictory! Both enemies have been defeated.");
                break;
            }
        }
        else {
            displayText("Invalid agent choice or agent is incapacitated.");
        }

        // Update cooldowns after each round
        updateCooldowns(team);

        // Enemy turns (skip if immobilised)
        if (!enemiesImmobilized) {
            int damage1 = rand() % 30 + 10;
            int damage2 = rand() % 30 + 10;

            if (enemy1Health > 0) enemyAttack(team, damage1, enemy1Name);
            if (enemy2Health > 0) enemyAttack(team, damage2, enemy2Name);
        }
        else {
            displayText("Both enemies are immobilised and skip their turn.");
            enemiesImmobilized = false; // Reset immobilisation after one turn
        }
    }

    // Check if all agents are dead
    bool allDead = true;
    for (const Agent& agent : team) {
        if (agent.health > 0) {
            allDead = false;
            break;
        }
    }

    if (allDead) {
        displayText("\nGame Over! All your agents have been defeated.");
    }

    return 0;
}
