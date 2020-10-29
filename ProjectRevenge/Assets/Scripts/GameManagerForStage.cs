using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManagerForStage : GameManager
{
    private Player player;
    public GameObject playerDie;

    protected void Awake()
    {
        player = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
    }

    protected void Update()
    {
        if (player.mbDead)
        {
            playerDie.SetActive(true);
        }
    }
}
