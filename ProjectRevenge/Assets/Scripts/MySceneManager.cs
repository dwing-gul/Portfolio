using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MySceneManager : MonoBehaviour
{
    private GameObject Boss;
    private GameObject[] Enemies;
    private int CurrentSceneNumber;
    private Scene CurrentScene;
    private int NextScene;

    private void Start()
    {
        Boss = GameObject.FindGameObjectWithTag("Boss");
        Enemies = GameObject.FindGameObjectsWithTag("Enemy");
        CurrentScene = SceneManager.GetActiveScene();
        CurrentSceneNumber = CurrentScene.buildIndex;
        NextScene = CurrentSceneNumber + 1;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.CompareTag("Player"))
        {
            int EnemyCheck = Enemies.Length;

            if (CurrentSceneNumber != 4)
            {
                for (int index = 0; index < Enemies.Length; index++)
                {
                    if (Enemies[index].activeSelf == false)
                    {
                        EnemyCheck--;
                    }
                }

                if (Boss.activeSelf == false && EnemyCheck == 0)
                {
                    SceneManager.LoadScene(NextScene);
                }
            }

            else
            {
                for (int index = 0; index < Enemies.Length; index++)
                {
                    if (Enemies[index].activeSelf == false)
                    {
                        EnemyCheck--;
                    }
                }

                if (EnemyCheck == 0)
                {
                    SceneManager.LoadScene(NextScene);
                }
            }
        }
    }
}
