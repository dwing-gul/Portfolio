using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FirePillarManagerController : MonoBehaviour
{
    public Player Target;
    public HellBeast Owner;
    public FirePillarController FirePillar;
    public FirePillarForetokenController Foretoken;

    private Transform targetPos;

    // Start is called before the first frame update
    void Start()
    {
        Target = GetComponent<Player>();
        Owner = GetComponent<HellBeast>();
        FirePillar = GetComponent<FirePillarController>();
        Foretoken = GetComponent<FirePillarForetokenController>();
    }

    public void MovePillar()
    {
        transform.position = new Vector3(Target.gameObject.transform.position.x, transform.position.y, 0);
    }

    public void ForeToken()
    { 
        if(Owner.mbAttack)
        {
            targetPos.position = new Vector3(Target.gameObject.transform.position.x, transform.position.y, 0);

            Foretoken.gameObject.SetActive(true);
            if(Foretoken.isUp)
            {
                Foretoken.gameObject.SetActive(false);
                FirePillar.gameObject.SetActive(true);
            }
        }
        else
        {
            FirePillar.gameObject.SetActive(false);
        }
    }
}
