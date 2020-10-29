using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public void GamePause(bool pause)
    {
        if (pause)
        {
            Time.timeScale = 0;
        }
        else
        {
            Time.timeScale = 1f;
        }
    }

    public void GameStart()
    {
        SceneManager.LoadScene(1);
    }

    public void GoMainWindow()
    {
        SceneManager.LoadScene(0);
    }
    public void GameExit()
    {
        Application.Quit();
    }
}
