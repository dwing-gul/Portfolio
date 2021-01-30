using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIButtonManager : MonoBehaviour
{
    public Player player;

    public void Init()
    {
        player = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
    }
    public void LeftUp()
    {
        player.inputLeft = false;
    }

    public void LeftDown()
    {
        player.inputLeft = true;
    }

    public void RightUp()
    {
       player.inputRight = false;
    }
    public void RightDown()
    {
        player.inputRight = true;
    }

    public void UpClickUp()
    {
        player.inputUp = false;
    }

    public void UpClickDown()
    {
        player.inputUp = true;
    }

    public void DownClickUp()
    {
        player.inputDown = false;
    }

    public void DownClickDown()
    {
        player.inputDown = true;
    }

    public void AttackClickUp()
    {
        player.inputAttack = false;
    }

    public void AttackClickDown()
    {
        player.inputAttack = true;
    }

    public void ShieldClickUp()
    {
        player.inputShield = false;
    }

    public void ShieldClickDown()
    {
        player.inputShield = true;
    }
}
