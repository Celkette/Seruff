/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_var_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 07:36:03 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/23 09:59:55 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/************************************************************
*      void check_local_var(t_env *minishell, char *s)      *
* 1. parse s en key et val (malloc par parse_var())         *
* 2. cherche si la variable existe deja dans minishell->var *
*       is_var_in_local()                                    *
* 3. si oui, supprime l'ancienne variable de la liste       *
*        ft_delete_node()                                   *
* 4. cree une nouvelle variable locale                      *
*        creat_local_var()                                  *
* 5. ajoute la nouvelle variable a la liste                 *
*        ft_add_node()                                      *
* 6. free key et val (malloc par parse_var)                 *
*************************************************************/

/***********************************************************
 * La variable est elle dans la liste des variable locale? *
 ***********************************************************/
//@Cherhe la variable var_name dans la liste des variables locales(minishell->var)
// @param list Pointeur vers la liste des variables locales
// @param s Chaine de la variable a rechercher (ex:"VAR")
// @param var Pointeur vers la structure t_var a remplir avec la valeur trouvee
// @return Pointeur vers la structure de la variable si trouvee, NULL sinon
t_var	*is_var_in_local(t_list *list, char *var_name)
{
	t_list	*current;
	t_var	*var_str;

	if (!list || !var_name)
		return (NULL);
	current = list;//current devient la tete de list
	while (current)
	{
		var_str = (t_var *)current->data;// Cast de current->data en t_var*
		if (var_str && var_str->key
			    && ft_strlen(var_name) == ft_strlen(var_str->key)
			    && ft_strncmp(var_name, var_str->key, ft_strlen(var_name)) == 0)
			return (var_str);// retourne struct de la var trouvee (struct, key, val)
			//return (var_str->val);//retourne la val de la variable
		current = current->next;
	}
	return (NULL); // Variable non trouvée
}

/*********************************************************
 * 	Decoupe la nouvelle variable locale NAME=celine	     *
 * 		pour la stocker dans une structure t_var 		 *
 * 			var->key=NAME et var->val=celine			 *
 *********************************************************/
//@parse la string en *key et *val qui seront alloués dynamiquement
// @param s Chaine de la variable a ajouter (ex:"VAR=value")
// @param key Pointeur vers la clé de la variable (ex:"VAR")
// @param val Pointeur vers la valeur de la variable (ex:"value")
// @return 0 si la variable est valide, -1 si la variable est invalide
// @note Si la variable est invalide (pas de '=' ou clé vide), retourne -1	
int	parse_var(char *s, char **key, char **val)
{
	int		key_len;// Longueur de la clé (jusqu'au '=')
	int		val_len;// Longueur de la valeur (après le '=')

	key_len = 0;
	val_len = 0;
	if (!s || !key || !val) //si vide ou NULL
		return (-1);
	if (!ft_strchr(s, '=') || s[0] == '=') //si ne trouve pas le '=' ou si '=' en 1er position
		return (-1);
	while (s[key_len] && s[key_len] != '=') //cherche l'index du '='
		key_len++;
	while (s[val_len + key_len + 1]) //cherche la fin de val (on part de '=' +1)(= len de val)
		val_len++;
//à ce niveau key_len est forcement > 0 et key_val >= 0 (si =0 cree chaine vide)
	//alloue et copie la clé	
	*key = ft_strndup(s, key_len);// ✅ Pointeur vers début								-->MALLOC
//ou var->key = ft_strndup(&s[0], key_len);// ✅ Pointeur vers position
	*val = ft_strndup((s + key_len + 1), val_len);// ✅ Pointeur vers début+key_len+1	-->MALLOC
//ou var->val = ft_strndup(&s[key_len + 1], val_len);// ✅ Pointeur vers position
	if (!*val || !*key)
	{
		free(*key);
		free(*val);
		return (-1);
	}
	return (0); // Retourne 0 si la variable est valide
}

/***********************************************************
 * Crée la structure t_var de la nouvelle variable locale  *
 * 			qui sera ajoutée dans la liste chainée	       *
 ***********************************************************/
//@Cree pour variable locale une stucture t_var et initialise ses champs key et val
// @param key Clé de la variable (ex:"NAME)
// @param val Valeur de la variable (ex:"celine")
// @return Pointeur vers la structure t_var si l'allocation réussit, NULL sinon
t_var	*creat_local_var(char *key, char *val)
{
	t_var	*var;
	//allocation pour la struct et ses variables(tout a la suite)
	var = malloc(sizeof(t_var) + ft_strlen(key) + 1 + ft_strlen(val) + 1);//              --> MALLOC
	if (!var)
		return (NULL);
	//var pointe automatiquement vers la structure
	//var->key pointe vers apres var
	var->key = (char *)var + sizeof(t_var);
	//var->val pointe vers apres var->key
	var->val = (char *)var + sizeof(t_var) + ft_strlen(key) + 1;
	//ft_strcpy(var->key, key);
	//ft_strcpy(var->val, val);
	ft_memcpy(var->key, key, ft_strlen(key) + 1);//copie PATH a l'adresse var->key
	ft_memcpy(var->val, val, ft_strlen(val) + 1);//copie /usr/bin a l'adresse var->val
	return (var);
}

//@Ajoute ou met a jour une variable locale dans la liste des variables du minishell
//@param minishell Pointeur vers la structure d'environnement du shell
//@param s Chaine de la variable a ajouter (ex:"VAR=value")
//@return 0 si la variable est valide, -1 si la variable est invalide
// @ note Si la variable est invalide (pas de '=' ou clé vide), rien n'est fait
// @note Si la variable existe deja, sa valeur est mise a jour
// @ note Si la variable n'existe pas,struct créee et ajoutée a la liste des variables
// @ note Si la variable est vide (ex:"VAR="), elle est creee et ajoutée avec une empty value
void	update_local_var(t_env *minishell, char *s)
{
	char	*key;
	char	*val;
	t_var	*exist_var;
	t_var	*new_var;

	if (!minishell || !s)
		return ;
	if (parse_var(s, &key, &val) != 0)
		return ;
	exist_var = is_var_in_local(minishell->var, key);//pointeur vers la variable locale existante ou NULL si inexistante
	if (exist_var) //si s est  une variable locale existante
	{
	// Supprimer l'ancienne variable de la liste
		minishell->var = ft_delete_node(minishell->var);//supprime l'ancienne variable de la liste
	// Créer une nouvelle avec les nouvelles valeurs
		new_var = creat_local_var(key, val);
	// L'ajouter à la liste
		minishell->var = ft_add_node(minishell->var, new_var);
	}
	else//si n'est pas une variable locale existante
	{
		new_var = creat_local_var(key, val);//cree la structure de la variable locale
		if (new_var)
			minishell->var = ft_add_node(minishell->var, new_var);//ajoute la nouvelle variable a la liste
	}
	free(key);
	free(val);
}

/********************************************************************************
 * Libere mémoire allouée pour chaque variable (struct, clé et valeur) et noeud *
 * 	util pour -À la fermeture du shell (obligatoire)	       	        	    *
 *			  -En cas d'erreur fatale (cleanup)	       	     			   	    *
 *			  -Dans les tests (pour valgrind)	       	        			    *
 *            -Reset complet des variables (si besoin)	       	        	    *
 ********************************************************************************/
//@Libere mémoire allouée pour liste chainée des variables locales avant de quitter le shell
// @param minishell Pointeur vers la structure d'environnement du shell
// Cette fonction parcourt la liste des variables d'environnement, libère la mémoire
// allouée pour chaque variable (clé et valeur), ainsi que pour le noeud de la liste.
void	clean_var(t_env *minishell)
{
	t_list	*to_free;
	t_var	*var;

	if (!minishell || !minishell->var)
		return ;
	while (minishell->var)
	{
		var = (t_var *)minishell->var->data;
			free(var);// Libère la structure de la variable
		to_free = minishell->var;
		minishell->var = minishell->var->next;
		free(to_free);
	}
}

/*
📊 Comparaison des performances
Approche	Allocations                Parcours liste	  Libérations

Actuelle	Toujours 3 malloc	       1 parcours	      3 free si existe

Optimisée	Seulement si nécessaire	   1 parcours	      Minimal


🎯 Avantages de la version optimisée
Moins d'allocations : Pas de structure temporaire
Plus claire : Logique linéaire (parse → cherche → agit)
Plus rapide : Évite les allocations/libérations inutiles
Plus maintenable : Chaque fonction a un rôle précis
Moins de bugs : Moins de gestion mémoire complexe

🤔 Votre approche actuelle

Avantages :
Fonctionne correctement
Code modulaire

Inconvénients :
Gaspillage mémoire/CPU
Logique complexe à suivre
Plus de risques de fuites mémoire
*/

/*****************************************
*		Fonctions de test et main	     *
*                                        *
* avec Makefile_test_dollar 	         *
* make -f Makefile_test                  * 
* ./test_dollar                          *
* make -f Makefile_test fclean           * 
******************************************/
/*
// Fonction pour afficher les variables de la liste
void print_test_variables(t_list *var_list)
{
    t_list *current = var_list;
    t_var *var;
    int i = 1;

    printf("\n=== LISTE CHAINEE VARIABLES LOCALES ===\n");
    if (!current)
    {
        printf("Aucune variable locale définie\n");
        return;
    }
    
    while (current)
    {
        var = (t_var *)current->data;
        printf("%d. key=\"%s\" / val= \"%s\"\n", i++, var->key, var->val);
        current = current->next;
    }
    printf("========================\n\n");
}

// Fonction pour libérer une variable avec allocation unique
void free_test_single_var(t_var *var)
{
    if (var)
        free(var); // Un seul free car allocation unique
}

// Fonction pour libérer toute la liste
void free_test_var_list(t_list *list)
{
    t_list *current = list;
    t_list *next;
    
    while (current)
    {
        next = current->next;
        free_test_single_var((t_var *)current->data);
        free(current);
        current = next;
    }
}

void test_parse_var(void)
{
    printf("\n🔧 Test de parse_var\n");
    char *key, *val;
    
    // Test 1 : Variable normale
    if (parse_var("TEST=valeur_test", &key, &val) == 0)
    {
        printf("✅ Parse réussi de:\"TEST=valeur_test\"\n key='%s', val='%s'\n", key, val);
        free(key);
        free(val);
    }
    else
        printf("❌ Parse échoué\n");
    
    // Test 2 : Variable vide
    if (parse_var("EMPTY=", &key, &val) == 0)
    {
        printf("✅ Parse variable vide:\"EMPTY=\"\n key='%s', val='%s'\n", key, val);
        free(key);
        free(val);
    }
    else
        printf("❌ Parse variable vide échoué\n");
    
    // Test 3 : Cas d'erreur
    if (parse_var("=erreur", &key, &val) != 0)
        printf("✅ Erreur détectée correctement pour \"=erreur\"\n");
    else
        printf("❌ Erreur non détectée\n");
}

void test_creat_local_var(void)
{
    printf("\n🔧 Test de creat_local_var\n");
    
    t_var *var = creat_local_var("TEST_KEY", "test_value");
    if (var)
    {
        printf("✅ Création réussie: %s = %s\n", var->key, var->val);
        
        // Vérifier l'allocation unique
        printf("🔍 Vérification allocation unique:\n");
        printf("   - Adresse structure: %p\n", (void *)var);
        printf("   - Adresse key: %p\n", (void *)var->key);
        printf("   - Adresse val: %p\n", (void *)var->val);
        printf("   - Différence key-struct: %ld octets\n", (char *)var->key - (char *)var);
        printf("   - sizeof(t_var): %zu octets\n", sizeof(t_var));
        
        free_test_single_var(var);
    }
    else
        printf("❌ Création échouée\n");
}

void is_var_in_local(t_list *list)
{
    printf("\n🔧 Test de is_var_in_local\n");
    
    t_var *found = is_var_in_local(list, "NAME");
    if (found)
        printf("✅ Variable NAME trouvée: %s\n", found->val);
    else
        printf("❌ Variable NAME non trouvée\n");
    
    found = is_var_in_local(list, "INEXISTANTE");
    if (found)
        printf("✅ Variable INEXISTANTE trouvée: %s\n", found->val);
    else
        printf("✅ Variable INEXISTANTE non trouvée (normal)\n");
}

#ifdef TEST_MODE
int main(void)
{
    // Simulation d'un environnement minishell minimal
    t_env minishell;
    minishell.var = NULL; // Pas de variables au début
    
	printf("======================================================\n");
    printf("🧪 TEST COMPLET DES FONCTIONS DE VARIABLES LOCALES 🧪\n");
    printf("======================================================\n\n");
    
    // Test 1: Ajouter une première variable
    printf("📝 Test 1: Ajout NAME=celine\n");
    set_local_var(&minishell, "NAME=celine");
    print_test_variables(minishell.var);
    
    // Test 2: Ajouter une deuxième variable
    printf("📝 Test 2: Ajout PATH=/usr/bin\n");
    set_local_var(&minishell, "PATH=/usr/bin");
    print_test_variables(minishell.var);
    
    // Test 3: Ajouter une variable vide
    printf("📝 Test 3: Ajout EMPTY=\n");
    set_local_var(&minishell, "EMPTY=");
    print_test_variables(minishell.var);
	
	//Ajouter une variable similaire avant de tester la mise à jour
    printf("📝 Test 3.1: Ajout NAMES=isidor\n");
	printf("📝 Test 3.1: Ajout NAM=orphee\n");
    set_local_var(&minishell, "NAMES=isidor");
	set_local_var(&minishell, "NAM=orphee");
    print_test_variables(minishell.var);
	
    // Test 4: Mettre à jour une variable existante
    printf("📝 Test 4: Mettre à jour NAME=nico\n");
    set_local_var(&minishell, "NAME=nico");
    print_test_variables(minishell.var);
    
    // Test 5: Tester la fonction is_var_in_local
    printf("📝 Test 5: Rechercher des variables\n");
    test_is_var_in_local_func(minishell.var);
    
    // Test 6: Tester des cas d'erreur
    printf("\n📝 Test 6: Cas d'erreur\n");
	printf("Ajout de key vide, \"=valeur\"\n");
    set_local_var(&minishell, "=valeur"); // Clé vide
	printf("Ajout sans '=', \"VAR_SANS_EGAL\"\n");
    set_local_var(&minishell, "VAR_SANS_EGAL"); // Pas de '='
	printf("Ajout de string NULL\n");
    set_local_var(&minishell, NULL); // String NULL
    print_test_variables(minishell.var);
    
    // Test 7: Tests des fonctions individuelles
    printf("📝 Test 7: Tests des fonctions individuelles\n");
    test_parse_var();
    test_creat_local_var();
    
    printf("\n🎯 FIN DES TESTS\n");
    print_test_variables(minishell.var);
    
    // Nettoyage
    free_test_var_list(minishell.var);
    
    printf("✅ Tests terminés avec succès !\n");
    return 0;
}
#endif
*/